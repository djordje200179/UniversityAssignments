import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";
import {StudentInfo, TeacherInfo} from "./users.service";

export interface Enrollment {
	subject: { name: string };
	teacher: TeacherInfo;
}

export interface Rating {
	comment: string;
	rating: number;
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

	public getTeacherStudents(teacherUsername: string) {
		return this.httpClient.get<StudentInfo[]>(`${TeachersService.SERVER_URL}/students/${teacherUsername}`);
	}

	public getAllSubjects() {
		return this.httpClient.get<{ name: string }[]>(`${TeachersService.SERVER_URL}/subjects`);
	}

	public addSubject(subjectName: string) {
		return this.httpClient.post(`${TeachersService.SERVER_URL}/subjects`, subjectName);
	}

	public blockTeacher(username: string) {
		return this.httpClient.put(`${TeachersService.SERVER_URL}/block/${username}`, null);
	}

	public acceptTeacher(username: string) {
		return this.httpClient.put(`${TeachersService.SERVER_URL}/accept/${username}`, null);
	}

	public getBiographyURL(username: string) {
		return `${(TeachersService.SERVER_URL)}/biography/${username}`;
	}

	public getTeacherRatings(username: string) {
		return this.httpClient.get<Rating[]>(`${TeachersService.SERVER_URL}/ratings/${username}`);
	}
}