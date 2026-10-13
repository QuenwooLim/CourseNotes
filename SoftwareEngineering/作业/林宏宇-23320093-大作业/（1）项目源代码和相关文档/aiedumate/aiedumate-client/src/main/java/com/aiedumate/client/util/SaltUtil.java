package com.aiedumate.client.util;

import java.util.Random;

public class SaltUtil {
    private static final String CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    private static final int SALT_LENGTH = 16;
    private static final Random random = new Random();

    public static String generateSalt() {
        StringBuilder salt = new StringBuilder();
        for (int i = 0; i < SALT_LENGTH; i++) {
            salt.append(CHARACTERS.charAt(random.nextInt(CHARACTERS.length())));
        }
        return salt.toString();
    }
}