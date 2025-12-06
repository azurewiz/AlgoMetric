package com.algometric.backend;

import org.springframework.stereotype.Component;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

@Component
public class JobQueue {
    // DSA Concept: Storing Complex Objects in a FIFO Queue
    private final Queue<Job> queue;

    public JobQueue() {
        this.queue = new ConcurrentLinkedQueue<>();
    }

    public void enqueue(Job job) {
        queue.add(job);
        System.out.println("LOG: Job added: " + job.getId());
    }

    public Job dequeue() {
        if (queue.isEmpty()) return null;
        return queue.poll();
    }

    public int getSize() {
        return queue.size();
    }
}