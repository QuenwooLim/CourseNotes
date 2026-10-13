package com.aiedumate.client;

import org.junit.Test;

import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import java.util.Base64;

public class KeyGeneratorUtil {
    @Test
    public void generateHS512Key() {
        try{
            KeyGenerator keyGen = KeyGenerator.getInstance("HmacSHA512");
            keyGen.init(512); // 明确指定512位
            SecretKey secretKey = keyGen.generateKey();
            String key= Base64.getEncoder().encodeToString(secretKey.getEncoded());
            System.out.println("key:" + key);
        } catch (Exception ex){
            ex.printStackTrace();
        }
    }
}
