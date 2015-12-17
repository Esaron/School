package com.jdn.tcp;

import java.net.InetAddress;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;


public class Frame {

    private static enum Protocol {
        IP,
        ARP,
        TCP,
        UDP,
        ICMP,
        IPV6
    };

    private static DateFormat df = new SimpleDateFormat("MMM d, yyyy HH:mm:ss.S z");

    private float seconds;
    private Date arrivalTime = new Date();
    private Protocol protocol;
    private InetAddress src;
    private InetAddress dest;
    private TcpFlags flags = new TcpFlags();

    public Frame() {}

    public float getSeconds() {
        return seconds;
    }

    public void setSeconds(float seconds) {
        this.seconds = seconds;
    }

    public Date getArrivalTime() {
        return arrivalTime;
    }

    public void setArrivalTime(String arrivalTime)
            throws ParseException {
        try {
            this.arrivalTime = df.parse(arrivalTime);
        }
        catch (ParseException e) {
            System.err.println("Unable to parse input date");
            throw e;
        }
    }

    public void setArrivalTime(Date arrivalTime) {
        this.arrivalTime = arrivalTime;
    }

    public Protocol getProtocol() {
        return protocol;
    }

    public void setProtocol(Protocol protocol) {
        this.protocol = protocol;
    }

    public InetAddress getSrc() {
        return src;
    }

    public void setSrc(InetAddress src) {
        this.src = src;
    }

    public InetAddress getDest() {
        return dest;
    }

    public void setDest(InetAddress dest) {
        this.dest = dest;
    }

    public TcpFlags getFlags() {
        return flags;
    }

    public void setFlags(TcpFlags flags) {
        this.flags = flags;
    }

}
