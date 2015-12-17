package com.jdn.analyze;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

/**
 * TrafficTracker takes a filename for a wireshark trace capture and performs some analysis
 * to answer the questions for HW2's wireshark component.
 */
public class TrafficTracker {

    /**
     * The txt wireshark trace capture file
     */
    private File file;
    /**
     * The number of sites in our input data
     */
    private int numSites;
    /**
     * The total number of DNS queries sent out
     */
    private int dnsResolutions;
    /**
     * The average number of DNS queries for all sites
     */
    private float avgDnsResolutions;
    /**
     * The number of sites that gave us multiple Answer RRs
     */
    private int sitesLoadBalanced;
    /**
     * The number of HTTP requests for which we got a 200 response
     */
    private int embeddedObjs;
    /**
     * The average number of embedded objects for all sites
     */
    private float avgEmbeddedObjs;
    /**
     * The map to track site to number of returned Answer RRs
     */
    private Map<String, Integer> siteToLB = new HashMap<String, Integer>();

    public TrafficTracker(String filename, List<String> sites)
            throws FileNotFoundException, IOException {
        file = new File(filename);
        this.numSites = sites.size();
        // Read the file into the content String
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            String line;
            // Super simple state machine to grab the info we want
            // Whether we're looking for the site name
            boolean findName = false;
            // The site name
            String name = "";
            boolean findAnswer = false;
            while ((line = br.readLine()) != null) {
                // Naive way of processing the lines
                // Search for lines indicating a DNS query is to follow
                if (line.contains(" DNS ") && !line.contains(" response ")) {
                    dnsResolutions++;
                    // Start looking for the name
                    findName = true;
                }
                // Search for lines containing the name string if we're finding the name
                else if (findName && line.contains("            Name: ")) {
                    // Get the name
                    String curName = line.substring(line.lastIndexOf(" ") + 1);
                    // Check our input data for the name
                    boolean contains = false;
                    for (String site : sites) {
                        if (site.contains(curName)) {
                            contains = true;
                        }
                    }
                    if (contains) {
                        // Use the current name if we find it
                        name = curName;
                        // Insert a value into the map for the name if we don't have one
                        Integer current = siteToLB.get(name);
                        if (current == null) {
                            siteToLB.put(name, 0);
                        }
                        // Start looking for the answer RRs
                        // By moving findAnswer outside of the if block, we can count
                        // the number of load balanced urls even for urls that
                        // are just related to the main one (assuming that the
                        // DNS responses follow the DNS requests in order)
                        findAnswer = true;
                        // Stop looking for name
                        findName = false;
                    }
                }
                // Search the line for the Answer RRs string if we're looking for it
                else if (findAnswer && line.contains("Answer RRs: ")) {
                    // Get the number of possible DNS servers to query
                    int balancers = Integer.parseInt(line.substring(line.lastIndexOf(" ") + 1));
                    // Increment our map value for the current site by the number of answers
                    siteToLB.put(name, siteToLB.get(name) + balancers);
                    // Stop looking for the answer RRs
                    findAnswer = false;
                }
                // Look for 200 responses to count downloaded objects
                else if (line.contains("    HTTP/1.1 200 OK\\r\\n")
                        || line.contains("    HTTP/1.0 200 OK\\r\\n")
                        || line.contains("    HTTP/1.1 301 Moved Permanently\\r\\n")
                        || line.contains("    HTTP/1.0 301 Moved Permanently\\r\\n")
                        || line.contains("    HTTP/1.1 302 Found\\r\\n")
                        || line.contains("    HTTP/1.0 302 Found\\r\\n")) {
                    embeddedObjs++;
                }
            }
            // Calculate our averages
            avgDnsResolutions = (float)dnsResolutions/this.numSites;
            avgEmbeddedObjs = (float)embeddedObjs/this.numSites;
            // Count the number of entries that have load balancer count values
            List<String> remove = new ArrayList<String>();
            for (Entry<String, Integer> entry : siteToLB.entrySet()) {
                if (entry.getValue() == 0) {
                    remove.add(entry.getKey());
                }
            }
            for (String key : remove) {
                siteToLB.remove(key);
            }
            sitesLoadBalanced = siteToLB.keySet().size();
        }
    }

    public File getFile() {
        return file;
    }

    public int getDnsResolutions() {
        return dnsResolutions;
    }

    public float getAvgDnsResolutions() {
        return avgDnsResolutions;
    }

    public int getSitesLoadBalanced() {
        return sitesLoadBalanced;
    }

    public float getAvgEmbeddedObjs() {
        return avgEmbeddedObjs;
    }

    public void printLBSites() {
        for (Entry<String, Integer> entry : siteToLB.entrySet()) {
            System.out.print(entry.getKey() + " : ");
            System.out.println(entry.getValue());
        }
    }
}