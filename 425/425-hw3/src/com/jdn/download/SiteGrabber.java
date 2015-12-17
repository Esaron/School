package com.jdn.download;

import java.io.IOException;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.select.Elements;

/**
 * SiteGrabber takes a url and fetches a list of "a" elements containing site urls as the href
 */
public class SiteGrabber {

    /**
     * The url from which to grab the sites
     */
    private String url;
    /**
     * The "a" element nodes of the sites
     */
    private Elements sites;

    /**
     * @param url The url to grab the sites from
     */
    public SiteGrabber(String url) {
        this.url = url;
    }

    /**
     * Retrieve the sites or return them if they've already been fetched
     * @return
     * @throws IOException
     */
    public Elements getSites()
            throws IOException {
        if (sites == null) {
            // Use Jsoup to get a Java {@link Document} from the top 1000 URL (Could be improved for any URL)
            Document doc = Jsoup.connect(url).get();
            // Get the "a" element nodes of the sites (Using the only identifiable info on the top 1000 site)
            sites = doc.select("a[target=_blank]");
        }
        return sites;
    }
}
