package com.algometric.backend;

public class Job {
    private String id;
    private String code;
    private String language;

    public Job(String id, String code, String language) {
        this.id = id;
        this.code = code;
        this.language = language;
    }

    // Getters
    public String getId() { return id; }
    public String getCode() { return code; }
    public String getLanguage() { return language; }
}
