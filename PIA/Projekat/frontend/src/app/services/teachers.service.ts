import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";
import {TeacherInfo} from "./users.service";

export interface Enrollment {
	subject: string;
	teacher: TeacherInfo;
}

@Injectable({
	providedIn: "root"
})
export class TeachersService {
	private static readonly SERVER_URL = "http://localhost:8080/teachers";

	public constructor(private readonly httpClient: HttpClient) {

	}

	public getAllEnrollments(isElementarySchool: boolean, schoolYear: number) {
		if (!isElementarySchool)
			schoolYear += 8;

		return this.httpClient.get<Enrollment[]>(`${TeachersService.SERVER_URL}?schoolYear=${schoolYear}`);
	}

	public getTeacherEnrollments(teacherUsername: string) {
		return this.httpClient.get<Enrollment[]>(`${TeachersService.SERVER_URL}/${teacherUsername}`);
	}


}