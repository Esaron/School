package com.jdn.proxy;
import java.io.IOException;
import java.net.ServerSocket;
import com.jdn.util.ExpirableHashMap;

public class HttpProxy {

    /**
     * The DNS cache and static TTL
     */
    private static final long DNS_TTL = 30000L;
    public static final ExpirableHashMap<String, String> DNS_CACHE =
            new ExpirableHashMap<String, String>(DNS_TTL);

    /**
     * Lock for keeping thread error logging together
     */
    public static final Object LOG_LOCK = new Object();

    /**
     * The port on which to listen
     */
    private static int port;

    /**
     * The ServerSocket on which to listen for connections
     */
    private static ServerSocket serverSocket;

    /**
     * Default constructor
     */
    public HttpProxy() {}

    /**
     * Main program
     * @param args Input arguments.  Expected to be of length 1 (port #).
     */
    public static void main(String args[]) {
        // Read port from args
        if (args.length != 1) {
            System.err.println("Usage: java proxyd <port number>");
            System.exit(1);
        }
        port = Integer.parseInt(args[0]);

        try {
            // Start listening on {@port}
            serverSocket = new ServerSocket(port);
            // Accept connections while the proxy is running
            new ConnectionListener(serverSocket).listen();
        }
        catch (IOException e) {
            synchronized (LOG_LOCK) {
                System.err.println("Error encountered while facilitating connection.");
                e.printStackTrace();
            }
        }
    }
}
