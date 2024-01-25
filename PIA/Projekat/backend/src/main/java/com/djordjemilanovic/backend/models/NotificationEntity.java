package com.djordjemilanovic.backend.models;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import lombok.*;

import java.sql.Timestamp;

@Setter
@Getter
@Entity
@EqualsAndHashCode
@NoArgsConstructor
@Table(name = "notifications", schema = "pia")
public class NotificationEntity {
	@Id
	@Column(name = "id")
	private int id;

	@Basic
	@Column(name = "receiver")
	private String receiverUsername;

	@Basic
	@Column(name = "seen")
	private boolean seen;

	@Basic
	@Column(name = "message")
	private String message;

	@Basic
	@Column(name = "created_at")
	private Timestamp createdAt;

	@ManyToOne
	@JsonIgnore
	@JoinColumn(name = "receiver", referencedColumnName = "username", nullable = false, insertable = false, updatable = false)
	private UserInfoEntity receiver;

	public NotificationEntity(UserInfoEntity receiver, String message) {
		this.receiver = receiver;
		this.receiverUsername = receiver.getUsername();
		this.message = message;
		this.seen = false;
		this.createdAt = new Timestamp(System.currentTimeMillis());
	}
}
