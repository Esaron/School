package com.jdn.proxy;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;


public class ConnectionListener {

    private static int MAX_THREADS = 200;

    /**
     * The thread pool for connections
     */
    private final ExecutorService connectionThreadPool = Executors.newFixedThreadPool(MAX_THREADS);

    /**
     * The {@see ServerSocket} on which to listen
     */
    private ServerSocket serverSocket;

    public ConnectionListener(ServerSocket serverSocket) {
        this.serverSocket = serverSocket;

        // Shutdown hook for the connection thread pool
        Runtime.getRuntime().addShutdownHook(new Thread() {
            public void run() {
                connectionThreadPool.shutdown();
            }
        });
    }

    public void listen()
            throws IOException {
        while (true) {
            // Get incoming connection
            Socket clientSocket = serverSocket.accept();
            // Instantiate lock to block on until the remote socket is ready for streaming
            Object lock = new Object();
            // Flag for monitoring connection close status
            AtomicBoolean eof = new AtomicBoolean(false);
            // Start the thread to stream data from client to remote server
            ClientToServerRunnable clientToServerRunnable =
                    new ClientToServerRunnable(lock, serverSocket, clientSocket);
            ServerToClientRunnable serverToClientRunnable =
                    new ServerToClientRunnable(lock, clientSocket, clientToServerRunnable);
            clientToServerRunnable.setServerToClientRunnable(serverToClientRunnable);
            connectionThreadPool.execute(clientToServerRunnable);
            // Start the thread to stream data back to the client from the remote server
            connectionThreadPool.execute(serverToClientRunnable);
        }
    }
}
