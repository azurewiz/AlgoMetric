package com.algometric.backend;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import java.util.UUID;
import java.util.Map;

@RestController
@RequestMapping("/api/queue")
public class QueueController {

    private final JobQueue jobQueue;
    private final BrainService brainService;
    private final AnalysisRepository repository;

    @Autowired
    public QueueController(JobQueue jobQueue, BrainService brainService, AnalysisRepository repository) {
        this.jobQueue = jobQueue;
        this.brainService = brainService;
        this.repository = repository;
    }

    // NEW: Accepts JSON body { "code": "...", "language": "java" }
    @PostMapping("/add")
    public String addJob(@RequestBody Map<String, String> payload) {
        String code = payload.get("code");
        String language = payload.getOrDefault("language", "java");
        String jobId = "JOB-" + UUID.randomUUID().toString().substring(0, 8);

        // Store the ACTUAL code in the queue object
        Job newJob = new Job(jobId, code, language);
        jobQueue.enqueue(newJob);

        return jobId; // Return just the ID
    }

    @GetMapping("/process")
    public String processJob() {
        Job job = jobQueue.dequeue();
        
        if (job == null) {
            return "Queue is empty! Submit code first.";
        }

        // Send the REAL code (from the job object) to the Brain
        System.out.println("LOG: Processing " + job.getId());
        String result = brainService.analyzeCode(job.getCode(), job.getLanguage());

        AnalysisResult dbEntry = new AnalysisResult(job.getId(), job.getCode(), result);
        repository.save(dbEntry);
        System.out.println("LOG: Saved Report to Database (ID: " + dbEntry.getId() + ")");

        return "<h1>Analysis for " + job.getId() + "</h1><p>" + result.replace("\n", "<br>") + "</p>";
    }
}