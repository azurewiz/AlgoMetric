package com.algometric.backend;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;

import java.util.HashMap;
import java.util.Map;

@Service
public class BrainService {

    @Value("${brain.api.url}")
    private String brainUrl;

    private final RestTemplate restTemplate;

    public BrainService() {
        this.restTemplate = new RestTemplate();
    }

    public String analyzeCode(String codeSnippet, String language) {
        String fullUrl = brainUrl + "/analyze";

        Map<String, String> requestBody = new HashMap<>();
        requestBody.put("code", codeSnippet);
        requestBody.put("language", language);

        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);
        // CRITICAL FIX: This header tells Ngrok "I am a developer, let me in!"
        headers.set("ngrok-skip-browser-warning", "true");

        HttpEntity<Map<String, String>> entity = new HttpEntity<>(requestBody, headers);

        try {
            System.out.println("LOG: Sending request to " + fullUrl); // Debug Log
            
            // We use Map.class to capture WHATEVER Python sends back
            Map apiResponse = restTemplate.postForObject(fullUrl, entity, Map.class);
            
            System.out.println("LOG: Raw Response from Brain: " + apiResponse); // Debug Log

            if (apiResponse != null) {
                if (apiResponse.containsKey("analysis")) {
                    return (String) apiResponse.get("analysis");
                } else if (apiResponse.containsKey("error")) {
                    return "BRAIN ERROR: " + apiResponse.get("error");
                } else if (apiResponse.containsKey("detail")) {
                    return "VALIDATION ERROR: " + apiResponse.get("detail");
                }
            }
            
            return "Error: Empty response or unknown format. Raw: " + apiResponse;
            
        } catch (Exception e) {
            e.printStackTrace();
            return "Connection Failed: " + e.getMessage();
        }
    }
}