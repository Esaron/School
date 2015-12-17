package com.jdn.analyze;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;


public class RTTCalculator {

    // The capture text file
    private File capFile;
    // A map to cache our RTT data
    private Map<String, Long> serverToRTT = new HashMap<String, Long>();
    // A map to cache our server RTT count for averaging
    private Map<String, Long> serverCount = new HashMap<String, Long>();

    public RTTCalculator(String capFileName)
            throws FileNotFoundException, IOException, ParseException {
        capFile = new File(capFileName);
        // Read the file into the content String
        try (BufferedReader br = new BufferedReader(new FileReader(capFile))) {
            String line;
            // Super simple state machine to grab the info we want
            // Whether we're looking for the arrival time
            boolean findStart = true;
            // Search string for time
            String timeKey = "Arrival Time: ";
            Date time = new Date();
            DateFormat df = new SimpleDateFormat("MMM d, yyyy HH:mm:ss.S z");
            // Client IP
            boolean findSrc = false;
            // Search string for src
            String srcKey = "Internet Protocol Version 4, Src: ";
            int srcKeyLen = srcKey.length();
            // The client IP
            String src = "";
            // SSL line
            boolean findSsl = false;
            // Search string for ssl
            String sslKey = "Handshake Protocol: ";
            // The destination
            boolean findDest = false;
            // The server IP
            String dest = "";
            // End time
            boolean findEnd = false;
            Date end = new Date();
            while ((line = br.readLine()) != null) {
                // Naive way of processing the lines
                // Search for arrival time (start)
                if (findStart && line.contains(timeKey)) {
                    String timeStr = line.substring(line.indexOf(":") + 2);
                    time = df.parse(timeStr);
                    findSrc = true;
                    findStart = false;
                }
                // Search for lines indicating we found the source
                else if (line.contains(srcKey)) {
                    if (findSrc) {
                        // Get the src IP based on line format
                        src = line.substring(srcKeyLen, line.indexOf("(") - 1);
                        dest = line.substring(line.lastIndexOf(":") + 2, line.lastIndexOf("(") - 1);
                        findSrc = false;
                        findSsl = true;
                    }
                    else if (findDest) {
                        String thisDest = line.substring(srcKeyLen, line.indexOf("(") - 1);
                        if (thisDest.equals(dest)) {
                            Long currentRTT = serverToRTT.get(dest);
                            long diff = end.getTime() - time.getTime();
                            if (currentRTT == null) {
                                serverToRTT.put(dest, diff);
                                serverCount.put(dest, 1L);
                            }
                            else {
                                Long count = serverCount.get(dest);
                                serverToRTT.put(dest, (diff + count * currentRTT) / count + 1);
                                serverCount.put(dest, count + 1);
                            }
                            findDest = false;
                            findSsl = true;
                        }
                    }
                    else {
                        // We aren't looking at good packet; skip
                    }
                }
                // Search for lines containing the ssl key if we're looking
                else if (findSsl && line.contains(sslKey)) {
                    // Get the name
                    String handshakeProtocol = line.substring(line.lastIndexOf(":") + 2);
                    if ("Client Hello".equals(handshakeProtocol)) {
                        // Next state, we found the first one
                        findEnd = true;
                        findSsl = false;
                    }
                    else if ("Server Hello Done".equals(handshakeProtocol)) {
                        // Reset, found the end
                        findStart = true;
                        findSsl = false;
                    }
                    else {
                        findStart = true;
                        findSsl = false;
                    }
                }
                else if (findEnd && line.contains(timeKey)) {
                    String timeStr = line.substring(line.indexOf(":") + 2);
                    end = df.parse(timeStr);
                    findEnd = false;
                    findDest = true;
                }
            }
        }
    }

    // Print the server RTTs
    public void printServerRTTs() {
        for (String key : serverToRTT.keySet()) {
            System.out.println("Server: " + key);
            System.out.println("RTT: " + serverToRTT.get(key));
        }
    }
}
