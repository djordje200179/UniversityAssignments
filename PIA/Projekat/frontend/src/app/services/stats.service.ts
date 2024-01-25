import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";

export interface WelcomePageCounters {
	numberOfStudents: number;
	numberOfTeachers: number;

	numberOfClassesLastWeek: number;
	numberOfClassesLastMonth: number;
}

export type Enrollments = { [key: string]: string[] };

export type SubjectAgeCounters = { [key: string]: { [key: string]: number } };
export type GenderCounters = { [key: string]: number };

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
}
