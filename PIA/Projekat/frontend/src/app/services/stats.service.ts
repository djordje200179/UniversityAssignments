import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";
import {TeacherInfo} from "./users.service";

export interface WelcomePageCounters {
	numberOfStudents: number;
	numberOfTeachers: number;

	numberOfClassesLastWeek: number;
	numberOfClassesLastMonth: number;
}

export type Enrollments = { [key: string]: string[] };

export type SubjectAgeCounters = { [key: string]: { [key: string]: number } };
export type GenderCounters = { [key: string]: number };

export interface TopTeacher {
	teacher: TeacherInfo;
	classesPerMonth: number[];
}

@Injectable({
	providedIn: "root"
})
export class StatsService {
	private static readonly SERVER_URL = "http://localhost:8080/stats";

	public constructor(private readonly httpClient: HttpClient) {

	}

	public getWelcomePageCounters() {
		return this.httpClient.get<WelcomePageCounters>(StatsService.SERVER_URL + "/counters");
	}

	public getEnrollments() {
		return this.httpClient.get<Enrollments>(StatsService.SERVER_URL + "/enrollments");
	}

	public getSubjectAgeCounters() {
		return this.httpClient.get<SubjectAgeCounters>(StatsService.SERVER_URL + "/subject-age");
	}

	public getStudentCountByGender() {
		return this.httpClient.get<GenderCounters>(StatsService.SERVER_URL + "/student-gender");
	}

	public getTeacherCountByGender() {
		return this.httpClient.get<GenderCounters>(StatsService.SERVER_URL + "/teacher-gender");
	}

	public getClassesPerDays() {
		return this.httpClient.get<number[]>(StatsService.SERVER_URL + "/classes-per-days");
	}

	public getTopTeachers() {
		return this.httpClient.get<TopTeacher[]>(StatsService.SERVER_URL + "/top-teachers");
	}

	public getClassStatuses() {
		return this.httpClient.get<{[key: string]: number}>(StatsService.SERVER_URL + "/class-statuses");
	}

	public getTeacherStatuses() {
		return this.httpClient.get<{[key: string]: number}>(StatsService.SERVER_URL + "/teacher-statuses");
	}
}
