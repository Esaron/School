package com.jdn.tcp;


public class TcpFlags {

    private static final int NONCE = 256;
    private static final int CWR = 128;
    private static final int ECN_ECHO = 64;
    private static final int URGENT = 32;
    private static final int ACK = 16;
    private static final int PUSH = 8;
    private static final int RESET = 4;
    private static final int SYN = 2;
    private static final int FIN = 1;

    private int flags;
    private boolean nonceSet;
    private boolean cwrSet;
    private boolean ecnEchoSet;
    private boolean urgentSet;
    private boolean ackSet;
    private boolean pushSet;
    private boolean resetSet;
    private boolean synSet;
    private boolean finSet;

    public TcpFlags() {}

    public TcpFlags(int flagsIn) {
        set(flagsIn);
    }

    public TcpFlags(String flagsStr) {
        set(Integer.parseInt(flagsStr, 2));
    }

    public void set(int flagsIn) {
        flags = flagsIn;
        nonceSet = (flags & NONCE) == NONCE;
        cwrSet = (flags & CWR) == CWR;
        ecnEchoSet = (flags & ECN_ECHO) == ECN_ECHO;
        urgentSet = (flags & URGENT) == URGENT;
        ackSet = (flags & ACK) == ACK;
        pushSet = (flags & PUSH) == PUSH;
        resetSet = (flags & RESET) == RESET;
        synSet = (flags & SYN) == SYN;
        finSet = (flags & FIN) == FIN;
    }

    public boolean isNonceSet() {
        return nonceSet;
    }

    public boolean isCwrSet() {
        return cwrSet;
    }

    public boolean isEcnEchoSet() {
        return ecnEchoSet;
    }

    public boolean isUrgentSet() {
        return urgentSet;
    }

    public boolean isAckSet() {
        return ackSet;
    }

    public boolean isPushSet() {
        return pushSet;
    }

    public boolean isResetSet() {
        return resetSet;
    }

    public boolean isSynSet() {
        return synSet;
    }

    public boolean isFinSet() {
        return finSet;
    }

}
