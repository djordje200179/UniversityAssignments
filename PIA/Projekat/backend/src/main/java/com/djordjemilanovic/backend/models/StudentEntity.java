package com.djordjemilanovic.backend.models;

import com.fasterxml.jackson.annotation.JsonValue;
import jakarta.persistence.*;
import lombok.*;
import org.hibernate.annotations.ColumnTransformer;

@Setter
@Getter
@EqualsAndHashCode
@Entity
@NoArgsConstructor
@Table(name = "students", schema = "pia")
public class StudentEntity {
	public enum SchoolType {
		ELEMENTARY, GYMNASIUM, SPECIALIZED, ART;

		@JsonValue
		public String toLowerCase() {
			return toString().toLowerCase();
		}
	}

	@Id
	@Column(name = "username")
	private String username;
	@Basic
	@Column(name = "school_type")
	@ColumnTransformer(read = "UPPER(school_type)", write = "LOWER(?)")
	@Enumerated(EnumType.STRING)
	private SchoolType schoolType;
	@Basic
	@Column(name = "school_year")
	private int schoolYear;

	@OneToOne
	@JoinColumn(name = "username", referencedColumnName = "username")
	private UserInfoEntity info;

	public StudentEntity(String username, SchoolType schoolType, int schoolYear) {
		this.username = username;
		this.schoolType = schoolType;
		this.schoolYear = schoolYear;
	}
}
