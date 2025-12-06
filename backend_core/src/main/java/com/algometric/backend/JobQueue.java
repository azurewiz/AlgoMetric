package com.algometric.backend;

import java.util.LinkedList;

// DSA Concept: Custom Queue Implementation
// We will use this to hold code analysis jobs before processing
public class JobQueue {
    private LinkedList<String> queue;
    
    public JobQueue() {
        this.queue = new LinkedList<>();
    }

    public void enqueue(String jobID) {
        // TODO: Implement Logic
        queue.addLast(jobID);
    }

    public String dequeue() {
        // TODO: Implement Logic
        if (queue.isEmpty()) return null;
        return queue.removeFirst();
    }
}