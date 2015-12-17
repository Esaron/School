package com.jdn.main;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import com.jdn.analyze.RTTCalculator;
import com.jdn.analyze.TrafficTracker;
import com.jdn.download.SiteGrabber;

public class Main {

    private static String TOP_1000 = "http://stuffgate.com/stuff/website/top-1000-sites";
    private static String CAPTURE_FILE = "src/assets/siteCap.txt";

    public static void main(String[] args)
            throws FileNotFoundException, IOException, ParseException {
        hw3();
    }

    /**
     * Retrieves the urls from the top 1000 site
     * @return The list of urls for my id
     * @throws IOException
     */
    private static List<String> getUrls()
            throws IOException {
        List<String> result = new ArrayList<String>();
        SiteGrabber grabber = new SiteGrabber(TOP_1000);
        Elements sites = grabber.getSites();
        // Get the 3rd, 103rd, 203rd, etc. (My student ID ends in 3)
        // Note: The "Elements" class apparently uses 1-based indexing
        int i = 3;
        while (i < sites.size()) {
            Element site = sites.get(i);
            String urlStr = site.absUrl("href");
            result.add(urlStr);
            i += 100;
        }
        return result;
    }

    /**
     * Download the sites (used while collecting wireshark trace data)
     * @param urls The urls to download
     * @throws IOException
     */
    private static void downloadUrls(List<String> urls)
            throws IOException {
      // Download the sites
      for (String url : urls) {
          Jsoup.connect(url).get();
      }
    }

    /**
     * HW2 method
     * @throws IOException 
     * @throws FileNotFoundException 
     */
    private static void hw2()
            throws FileNotFoundException, IOException {
        // The data from the call to getUrls() in array format
        String[] siteArray = new String[] {
            "http://youtube.com",
            "http://bbc.com",
            "http://feedly.com",
            "http://wordreference.com",
            "http://hurriyet.com.tr",
            "http://urdupoint.com",
            "http://hupu.com",
            // Was getting socket timeouts for nikkei.com. Using 704 (zing.vn) instead.
            // "http://nikkei.com",
            "http://zing.vn",
            "http://nydailynews.com",
            "http://ruten.com.tw"
        };
        List<String> urls = Arrays.asList(siteArray);

        // The downloadUrls(urls) method was used to collect wireshark trace data
        // This data was saved as text using wiresharks export functionality

        // This code analyzes the trace data
        TrafficTracker tracker = new TrafficTracker(CAPTURE_FILE, urls);
        System.out.println(tracker.getDnsResolutions());
        System.out.println(tracker.getAvgDnsResolutions());
        System.out.println(tracker.getSitesLoadBalanced());
        System.out.println(tracker.getAvgEmbeddedObjs());
        tracker.printLBSites();
    }

    private static void hw3()
            throws FileNotFoundException, IOException, ParseException {
        RTTCalculator calc = new RTTCalculator(CAPTURE_FILE);
        calc.printServerRTTs();
    }
}
