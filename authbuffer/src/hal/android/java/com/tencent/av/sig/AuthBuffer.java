package com.tencent.av.sig;

import android.os.Environment;
import android.util.Log;

import com.tencent.av.utils.SoUtil;

import java.io.File;
import java.util.ArrayList;

public class AuthBuffer {
    private static boolean mIsSoLoaded = false;
    private static AuthBuffer sAuthBuffer;

    private static void loadSo() {
        if (!mIsSoLoaded) {
            try {
                System.loadLibrary("stlport_shared");
                System.loadLibrary("qav_authbuff");
                mIsSoLoaded = true;
            } catch (UnsatisfiedLinkError e) {
                mIsSoLoaded = false;
                e.printStackTrace();
            }
        }
    }

    private AuthBuffer() {
    }

    public static AuthBuffer getInstance() {
        if (sAuthBuffer == null) {
            synchronized(AuthBuffer.class){
                if (sAuthBuffer == null) {
                    loadSo();
                    if (mIsSoLoaded)
                        sAuthBuffer = new AuthBuffer();
                }
            }
        }

        return sAuthBuffer;
    }

    public native byte[] genAuthBuffer(int appId, int roomId, String identifier, int accountType, String key, int expTime, int authBits);
}
