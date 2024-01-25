package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.StudentEntity;
import com.djordjemilanovic.backend.models.UserInfoEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.HashMap;

@Repository
public interface StudentsRepository extends JpaRepository<StudentEntity, String> {
}
