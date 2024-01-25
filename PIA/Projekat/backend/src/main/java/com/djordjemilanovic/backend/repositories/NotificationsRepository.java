package com.djordjemilanovic.backend.repositories;

import com.djordjemilanovic.backend.models.NotificationEntity;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.Collection;

public interface NotificationsRepository extends JpaRepository<NotificationEntity, Integer> {
	Collection<NotificationEntity> findAllByReceiverUsername(String username);
}
