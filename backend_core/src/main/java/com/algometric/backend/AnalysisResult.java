package com.algometric.backend;

import jakarta.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "analysis_reports")
public class AnalysisResult {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private String jobId;
    
    // Store large text (the code and the AI result)
    @Column(columnDefinition = "TEXT")
    private String codeSnippet;

    @Column(columnDefinition = "TEXT")
    private String aiResponse;

    private String complexity; // e.g., "O(N^2)"
    private LocalDateTime timestamp;

    // Constructor
    public AnalysisResult() {} // Required by JPA

    public AnalysisResult(String jobId, String code, String response) {
        this.jobId = jobId;
        this.codeSnippet = code;
        this.aiResponse = response;
        this.timestamp = LocalDateTime.now();
        this.complexity = extractComplexity(response);
    }

    // A simple helper to find "O(...)" in the text
    private String extractComplexity(String text) {
        if (text.contains("O(N^2)")) return "O(N^2)";
        if (text.contains("O(N)")) return "O(N)";
        if (text.contains("O(logN)")) return "O(logN)";
        if (text.contains("O(1)")) return "O(1)";
        return "Unknown";
    }

    // Getters
    public Long getId() { return id; }
    public String getComplexity() { return complexity; }
    // ... add other getters if needed
}
