package com.algometric.backend;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class TestController {

    // This endpoint checks if the server is alive
    // Computer Networks Concept: HTTP GET Request on Port 8080
    @GetMapping("/health")
    public String healthCheck() {
        return "AlgoMetric Core System is ONLINE. Ready for Analysis.";
    }
}
