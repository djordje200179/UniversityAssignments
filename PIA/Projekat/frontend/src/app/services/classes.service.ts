import {StudentInfo, TeacherInfo} from "./users.service";
import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";

export interface ScheduleRequest {
	student: string;
	teacher: string;
	subject: string;
	topic: string;
	isDouble: boolean;
	time: Date;
}

export interface ClassInfo {
	id: number;
	teacherUsername: string,
	studentUsername: string,
	subject: string,
	topic: string,
	time: Date,
	confirmed: boolean,
	cancelled: boolean,
	teacherComment: string | null,
	studentComment: string | null,
	studentRating: number | null,
	teacherRating: number | null,
	teacher: TeacherInfo,
	student: StudentInfo
	double: boolean
}

@Injectable({
	providedIn: "root"
})
export class ClassesService {
	private static readonly SERVER_URL = "http://localhost:8080/classes";

	public constructor(private readonly httpClient: HttpClient) {

	}

	public scheduleClass(request: ScheduleRequest) {
		return this.httpClient.post(
			`${ClassesService.SERVER_URL}/schedule`, request,
			{responseType: "text"}
		);
	}

	public getArchive(username: string) {
		return this.httpClient.get<ClassInfo[]>(`${ClassesService.SERVER_URL}/archive/${username}`);
	}

	public getUpcoming(username: string) {
		return this.httpClient.get<ClassInfo[]>(`${ClassesService.SERVER_URL}/upcoming/${username}`);
	}

	public cancelClass(id: number, reason: string) {
		return this.httpClient.post<any>(`${ClassesService.SERVER_URL}/cancel/${id}`, reason);
	}

	public acceptClass(id: number) {
		return this.httpClient.post<any>(`${ClassesService.SERVER_URL}/accept/${id}`, {});
	}

	public getRequested(username: string) {
		return this.httpClient.get<ClassInfo[]>(`${ClassesService.SERVER_URL}/requested/${username}`);
	}

	public getAllTeacherClasses(username: string) {
		return this.httpClient.get<ClassInfo[]>(`${ClassesService.SERVER_URL}/teacher/${username}`);
	}

	public getStudentRating(username: string) {
		return this.httpClient.get(
			`${ClassesService.SERVER_URL}/student/${username}/rating`,
			{responseType: "text"}
		);
	}
}
