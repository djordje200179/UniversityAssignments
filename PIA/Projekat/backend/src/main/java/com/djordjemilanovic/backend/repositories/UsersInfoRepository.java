package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.UserInfoEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.Optional;

@Repository
public interface UsersInfoRepository extends JpaRepository<UserInfoEntity, String> {
	Optional<UserInfoEntity> findByEmailAddress(String emailAddress);
	Optional<UserInfoEntity> findByUsernameAndSecurityAnswer(String username, String securityAnswer);
}
