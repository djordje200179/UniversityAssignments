import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";
import {Observable} from "rxjs";

export interface Counters {
	numberOfStudents: number;
	numberOfTeachers: number;

	numberOfClassesLastWeek: number;
	numberOfClassesLastMonth: number;
}

export type Enrollments = { [key: string]: string[] };

@Injectable({
	providedIn: "root"
})
export class StatsService {
	private static readonly SERVER_URL = "http://localhost:8080/stats";

	public constructor(private readonly httpClient: HttpClient) {

	}

	public getCounters() {
		return this.httpClient.get<Counters>(StatsService.SERVER_URL + "/counters");
	}

	public getEnrollments() {
		return this.httpClient.get<Enrollments>(StatsService.SERVER_URL + "/enrollments");
	}
}
