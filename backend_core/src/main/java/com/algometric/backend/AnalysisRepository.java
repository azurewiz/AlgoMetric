package com.algometric.backend;
import org.springframework.data.jpa.repository.JpaRepository;
public interface AnalysisRepository extends JpaRepository<AnalysisResult, Long> {
}
