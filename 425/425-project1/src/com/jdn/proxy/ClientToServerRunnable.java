package com.jdn.proxy;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class ClientToServerRunnable implements Runnable {

    /**
     * The length of byte buffers to use when streaming data back to client
     */
    private static final int BUFFER_LENGTH = 4096;

    private static Pattern requestLinePattern =
            Pattern.compile("(?:OPTIONS|GET|HEAD|POST|PUT|DELETE|TRACE|CONNECT) " +
                    "(?:\\w+://)?" +
                    "([0-9A-Za-z](?:(?:[0-9A-Za-z]|-){0,61}[0-9A-Za-z])?(?:\\.[0-9A-Za-z](?:(?:[0-9A-Za-z]|-){0,61}[0-9A-Za-z])?)*\\.?)" +
                    "(?::(\\d+))?" +
                    "(?:\\S*) " +
                    "HTTP/(\\d\\.\\d)");

    /**
     * The byte buffer to use for streaming data back to the client
     */
    private byte[] buffer = new byte[BUFFER_LENGTH];

    /**
     * Whether or not the client connection has been closed
     */
    private boolean eof;

    /**
     * The {@see Object} used in corresponding {@see ServerToClientThread} to notify on
     */
    private Object lock;

    /**
     * The {@see ServerSocket} on which the proxy is listening
     */
    private ServerSocket serverSocket;

    /**
     * The {@see Socket} for the client
     */
    private Socket clientSocket;

    /**
     * The {@see Socket} for the remote server
     */
    private Socket remoteSocket;

    /**
     * The corresponding {@see ServerToClientRunnable} that is reading from the remote server socket
     */
    private ServerToClientRunnable serverToClientRunnable;

    public ClientToServerRunnable(Object lock, ServerSocket serverSocket, Socket clientSocket) {
        this.lock = lock;
        this.serverSocket = serverSocket;
        this.clientSocket = clientSocket;
    }

    /**
     * Get the {@see Socket} for the remote server
     * @return the remote {@see Socket}
     */
    public Socket getRemoteSocket() {
        return remoteSocket;
    }

    /**
     * Get client data status
     * @return whether eof is true (client socket is/will be closed)
     */
    public boolean isClientEof() {
        return eof;
    }

    /**
     * Associates the provided {@see ServerToClientRunnable} with this thread
     * @param serverToClientRunnable
     */
    public void setServerToClientRunnable(ServerToClientRunnable serverToClientRunnable) {
        this.serverToClientRunnable = serverToClientRunnable;
    }

    /**
     * Signal this task to stop
     */
    public void end() {
        eof = true;
        serverToClientRunnable.end();
    }

    //    /**
    //     * Returns the nth occurrence of c in str
    //     *
    //     * @param str
    //     *          The string
    //     * @param c
    //     *          The character to search for
    //     * @param n
    //     *          The numbered occurrence (left to right)
    //     * @return index
    //     *          The index of the nth occurrence
    //     */
    //    private int nthIndexOf(String str, char c, int n) {
    //        int index = str.indexOf(c);
    //        while (--n > 0 && index != -1) {
    //            index = str.indexOf(c, index+1);
    //        }
    //        return index;
    //    }

    /**
     * Parses requests to establish a remote server {@see Socket}, adds an X-Forwarded-For header,
     * and streams data from the client to the server.
     */
    @Override
    public void run() {
        String proxyIP = ((InetSocketAddress) clientSocket.getLocalSocketAddress()).getAddress().getHostAddress();
        int proxyPort = serverSocket.getLocalPort();
        // Get client {@see InputStream} if possible
        InputStream fromClient = null;
        try {
            fromClient = clientSocket.getInputStream();
        }
        catch (IOException e) {
            end();
            synchronized (HttpProxy.LOG_LOCK) {
                System.err.println(this + " Error while retrieving clientSocket InputStream.");
                e.printStackTrace();
            }
        }

        // Instantiate a {@see BufferedReader} for the client {@see InputStream}
        BufferedReader clientReader = new BufferedReader(new InputStreamReader(fromClient));
        OutputStream toRemote = null;

        // Continue streaming data as long as the connection is open
        while (!eof) {
            // The new altered request string and header variables
            StringBuilder newRequestBuilder = new StringBuilder();
            String remoteHost = "";
            Integer remotePort = null;
            String protocol = "";
            long contentLength = -1;
            boolean viaExists = false;

            // Track whether we think we should get a the request line from the headers
            boolean expectingRequestLine = true;
            try {
                // Read the headers
                String headerLine = null;
                while ((headerLine = clientReader.readLine()) != null) {
                    if (headerLine.isEmpty()) {
                        // Done reading headers
                        break;
                    }

                    // Expected format:
                    // <METHOD> <URL> HTTP/<VERSION>
                    // <Header>: <Value>
                    // etc.

                    // See if the pattern matches the request line
                    if (expectingRequestLine) {
                        Matcher requestLineMatcher = requestLinePattern.matcher(headerLine);
                        if (requestLineMatcher.matches()) {
                            // Extract the remote host
                            remoteHost = requestLineMatcher.group(1);
                            // Extract the remote port
                            String remotePortStr = requestLineMatcher.group(2);
                            if (remotePortStr != null) {
                                remotePort = Integer.parseInt(remotePortStr);
                            }
                            protocol = requestLineMatcher.group(3);
                            expectingRequestLine = false;
                        }
                    }
                    else if (headerLine.startsWith("Host: ")) {
                        // Extract remote host (favor header)
                        remoteHost = headerLine.substring(6);
                        // Extract remote port (favor header)
                        int colonIdx = remoteHost.indexOf(":");
                        if (colonIdx != -1) {
                            remotePort = Integer.parseInt(remoteHost.substring(colonIdx + 1));
                            remoteHost = remoteHost.substring(0, colonIdx);
                        }
                    }
                    else if (headerLine.startsWith("Content-Length: ")) {
                        // Extract content length
                        contentLength = Long.parseLong(headerLine.substring(16));
                    }
                    else if (headerLine.startsWith("Via: ")) {
                        headerLine = headerLine + ", " + protocol + " " + proxyIP + ":" + proxyPort;
                        viaExists = true;
                    }
                    // Append current header line
                    newRequestBuilder.append(headerLine)
                    .append("\r\n");
                }
                if (headerLine == null) {
                    // Connection closed - bail out and let the thread terminate
                    end();
                    break;
                }
                // Append X-Forwarded-For and Via headers and final \r\n
                newRequestBuilder.append("X-Forwarded-For: ")
                .append(clientSocket.getInetAddress().getHostAddress())
                .append(", ")
                .append(proxyIP)
                .append("\r\n");
                if (!viaExists) {
                    newRequestBuilder.append("Via: ")
                    .append(protocol)
                    .append(" ")
                    .append(proxyIP)
                    .append(":")
                    .append(proxyPort)
                    .append("\r\n");
                }
                newRequestBuilder.append("\r\n");
                // Retrieve the unexpired cached IP from the DNS cache or cache the lookup
                String cachedRemote = HttpProxy.DNS_CACHE.get(remoteHost);
                if (cachedRemote != null) {
                    remoteHost = cachedRemote;
                }
                else {
                    String remoteIP = InetAddress.getByName(remoteHost).getHostAddress();
                    HttpProxy.DNS_CACHE.put(remoteHost, remoteIP);
                    remoteHost = remoteIP;
                }
            }
            catch (IOException e) {
                end();
                synchronized (HttpProxy.LOG_LOCK) {
                    System.err.println(this + " Error while reading data from client.");
                    e.printStackTrace();
                }
                break;
            }

            String newRequestStr = newRequestBuilder.toString();

            try {
                // Establish the connection to the remote host if we don't have one
                if (remoteSocket == null) {
                    // Use default port if null
                    if (remotePort == null) {
                        remotePort = 80;
                    }
                    // Create a socket for the destination host
                    remoteSocket = new Socket(remoteHost, remotePort);
                    // Wake the ServerToClientThread up now that the remoteSocket exists
                    synchronized (lock) {
                        lock.notifyAll();
                    }
                    toRemote = remoteSocket.getOutputStream();
                }

                // Stream the new request headers to the server
                toRemote.write(newRequestStr.getBytes());
                toRemote.flush();
            }
            catch (IOException e) {
                remoteSocket = null;
                synchronized (HttpProxy.LOG_LOCK) {
                    System.err.println(this + " Error while writing data to remote server.");
                    e.printStackTrace();
                }
            }
            // Stream the rest of the content if any exists
            if (contentLength == -1) {
                // No Content-Length header present.  Stream until connection closed.
                while (!eof) {
                    // Stream data from the remote server back to the client
                    int bytesRead = -1;
                    try {
                        bytesRead = fromClient.read(buffer, 0, BUFFER_LENGTH);
                    }
                    catch (IOException e) {
                        synchronized (HttpProxy.LOG_LOCK) {
                            System.err.println(this + " Error while reading data from server.");
                            e.printStackTrace();
                        }
                        end();
                        break;
                    }
                    if (bytesRead == -1) {
                        // Connection closed.  Bail out.
                        end();
                        break;
                    }
                    try {
                        toRemote.write(buffer, 0, bytesRead);
                        toRemote.flush();
                    }
                    catch (IOException e) {
                        synchronized (HttpProxy.LOG_LOCK) {
                            System.err.println(this + " Error while sending data to client.");
                            e.printStackTrace();
                        }
                        end();
                    }
                }
            }
            else {
                for (int bytesRead = 0; !eof && bytesRead < contentLength; bytesRead++) {
                    int nextByte = -1;
                    try {
                        nextByte = fromClient.read();
                    }
                    catch (IOException e) {
                        end();
                        synchronized (HttpProxy.LOG_LOCK) {
                            System.err.println(this + " Error while reading data from client.");
                            e.printStackTrace();
                        }
                        break;
                    }
                    if (nextByte == -1) {
                        // Connection closed - bail out and let the thread terminate
                        end();
                        break;
                    }
                    try {
                        toRemote.write(nextByte);
                        toRemote.flush();
                    }
                    catch (IOException e) {
                        remoteSocket = null;
                        synchronized (HttpProxy.LOG_LOCK) {
                            System.err.println(this + " Error while writing data to remote server.");
                            e.printStackTrace();
                        }
                    }
                }
            }
        }
        end();
        try {
            if (clientSocket != null) {
                clientSocket.close();
            }
        }
        catch (IOException e) {
            synchronized (HttpProxy.LOG_LOCK) {
                System.err.println(this + " Error while closing client socket.");
                e.printStackTrace();
            }
        }
        finally {
            // Wake the other thread so it can terminate
            synchronized (lock) {
                lock.notifyAll();
            }
        }
    }
}
