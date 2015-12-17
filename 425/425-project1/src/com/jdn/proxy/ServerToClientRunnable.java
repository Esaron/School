package com.jdn.proxy;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;


public class ServerToClientRunnable implements Runnable {

    /**
     * The length of byte buffers to use when streaming data back to client
     */
    private static final int BUFFER_LENGTH = 4096;

    /**
     * The {@see Object} used in corresponding {@see ServerToClientThread} to notify on
     */
    private Object lock;

    /**
     * Whether or not the connection has been closed
     */
    private volatile boolean eof;

    /**
     * The {@see Socket} for the client
     */
    private Socket clientSocket;

    /**
     * The corresponding {@see ClientToServerRunnable} that has established the remote server connection
     */
    ClientToServerRunnable clientToServerRunnable;

    /**
     * The byte buffer to use for streaming data back to the client
     */
    private byte[] buffer = new byte[BUFFER_LENGTH];

    public ServerToClientRunnable(Object lock, Socket clientSocket, ClientToServerRunnable remoteSocketThread) {
        this.lock = lock;
        this.clientSocket = clientSocket;
        this.clientToServerRunnable = remoteSocketThread;
    }

    public void end() {
        eof = true;
    }

    /**
     * Streams data from the remote server back to the client as long as the connection remains open
     */
    @Override
    public void run() {
        // Wait for the remote connection to be established before attempting to get the remote server
        // {@see Socket}
        synchronized (lock) {
            try {
                lock.wait();
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // Get the remote server {@see Socket}
        Socket remoteSocket = clientToServerRunnable.getRemoteSocket();
        if (clientToServerRunnable.isClientEof() || remoteSocket == null) {
            // Something bad happened with the {@see ClientToServerThread} - bail out to avoid a memory
            // leak
            return;
        }
        OutputStream toClient = null;
        InputStream fromRemote = null;
        try {
            // Get the client {@see Socket} {@see OutputStream} and the remote server {@see Socket}
            // {@see InputStream}
            toClient = clientSocket.getOutputStream();
            fromRemote = remoteSocket.getInputStream();
        }
        catch (IOException e) {
            synchronized (HttpProxy.LOG_LOCK) {
                System.err.println(this + " Error while retrieving streams from sockets.");
                e.printStackTrace();
            }
            end();
        }

        while (!eof && !clientToServerRunnable.isClientEof()) {
            // Stream data from the remote server back to the client
            int bytesRead = -1;
            try {
                bytesRead = fromRemote.read(buffer, 0, BUFFER_LENGTH);
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
                toClient.write(buffer, 0, bytesRead);
                toClient.flush();
            }
            catch (IOException e) {
                synchronized (HttpProxy.LOG_LOCK) {
                    System.err.println(this + " Error while sending data to client.");
                    e.printStackTrace();
                }
                end();
            }
        }
        try {
            clientToServerRunnable.end();
            if (remoteSocket != null) {
                remoteSocket.close();
            }
        }
        catch (IOException e) {
            synchronized (HttpProxy.LOG_LOCK) {
                System.err.println(this + " Error while closing client socket.");
                e.printStackTrace();
            }
        }
    }
}
