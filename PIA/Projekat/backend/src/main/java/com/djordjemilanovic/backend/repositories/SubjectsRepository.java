package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.SubjectEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface SubjectsRepository extends JpaRepository<SubjectEntity, String> {
}
