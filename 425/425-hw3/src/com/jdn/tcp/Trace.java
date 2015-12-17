package com.jdn.tcp;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class Trace {

    private static enum State {
        NEW,
        INIT,
        ARRIVAL,
        PROTOCOL,
        SRC_DEST,
        FLAGS
    };

    private static String interfaceOrIp = "(?:\\w+_[\\d|\\w]{2}:[\\d|\\w]{2}:[\\d|\\w]{2}|\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})";
    private static Pattern frameHeaderPattern = Pattern.compile("\\s+\\d+\\s+(\\d+\\.\\d+)\\s+" + interfaceOrIp + "\\s+" + interfaceOrIp);

    private static Pattern arrivalPattern = Pattern.compile("\\s+Arrival Time: ([^\n]+)$");

    private State state = State.NEW;

    private Frame current;

    private File capFile;
    private List<Frame> frames = new ArrayList<Frame>();

    public Trace(String filename)
            throws IOException {
        this(new File(filename));
    }

    public Trace(File file)
            throws IOException {
        capFile = file;
        try {
            parseFrames();
        }
        catch (IOException e) {
            System.err.println("Error parsing input tcp trace file");
            throw e;
        }
    }

    private void parseFrames()
            throws IOException, ParseException {
        try (BufferedReader br = new BufferedReader(new FileReader(capFile))) {
            String line;
            while ((line = br.readLine()) != null) {
                switch(state) {
                    case NEW:
                        Matcher frameHeaderMatcher = frameHeaderPattern.matcher(line);
                        if (frameHeaderMatcher.matches()) {
                            Frame frame = new Frame();
                            frame.setSeconds(Float.parseFloat(frameHeaderMatcher.group(1)));
                            current = frame;
                            frames.add(current);
                            state = State.INIT;
                        }
                        break;
                    case INIT:
                        Matcher arrivalMatcher = arrivalPattern.matcher(line);
                        if (arrivalMatcher.matches()) {
                            current.setArrivalTime(arrivalMatcher.group(1));
                            state = State.ARRIVAL;
                        }
                        break;
                    case ARRIVAL:
                        break;
                    case PROTOCOL:
                        break;
                    case SRC_DEST:
                        break;
                    case FLAGS:
                        break;
                }
            }
        }
    }

}
