import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";

export interface Credentials {
	username: string;
	password: string;
}

export enum Role {
	Student = "student",
	Teacher = "teacher",
}

export enum Gender {
	Female = "female",
	Male = "male"
}

export interface UserInfo {
	username?: string;
	role?: Role;

	firstName: string;
	lastName: string;
	gender: Gender;

	securityQuestion?: string;
	securityAnswer?: string;

	address: string;
	phoneNumber: string;
	emailAddress: string;

	profileImage?: File;
	profileImageURL?: string;
}

export enum SchoolType {
	Elementary = "elementary",
	Gymnasium = "gymnasium",
	Specialized = "specialized",
	Art = "art"
}

export interface StudentInfo {
	credentials: Credentials;
	info: UserInfo;

	schoolType: SchoolType;
	schoolYear: number;
}

export interface TeacherInfo {
	credentials?: Credentials;
	info: UserInfo;

	biography?: File;
	subjects: string[];

	teachesLowerElementary: boolean;
	teachesUpperElementary: boolean;
	teachesHigh: boolean;
}

@Injectable({
	providedIn: "root"
})
export class UsersService {
	private static readonly SERVER_URL = "http://localhost:8080/users";

	public constructor(private readonly httpClient: HttpClient) {

	}

	public signIn(credentials: Credentials) {
		return this.httpClient.post<UserInfo | null>(`${(UsersService.SERVER_URL)}/sign-in`, credentials);
	}

	public signUpStudent(studentInfo: StudentInfo) {
		return this.httpClient.post<StudentInfo>(`${(UsersService.SERVER_URL)}/sign-up/student`, studentInfo);
	}

	public signUpTeacher(teacherInfo: TeacherInfo) {
		return this.httpClient.post<TeacherInfo>(`${(UsersService.SERVER_URL)}/sign-up/teacher`, teacherInfo);
	}

	public getCurrentUser() {
		const json = localStorage.getItem("user-info");
		if (json === null)
			return null;

		return JSON.parse(json) as UserInfo;
	}

	public getCurrentUserStudentInfo() {
		const json = localStorage.getItem("student-info");
		if (json === null)
			return null;

		return JSON.parse(json) as StudentInfo;
	}

	public getCurrentUserTeacherInfo() {
		const json = localStorage.getItem("teacher-info");
		if (json === null)
			return null;

		return JSON.parse(json) as TeacherInfo;
	}

	public signOut() {
		localStorage.removeItem("user-info");
	}

	public getStudentInfo(username: string) {
		return this.httpClient.get<StudentInfo>(`${(UsersService.SERVER_URL)}/student/${username}`);
	}

	public getTeacherInfo(username: string) {
		return this.httpClient.get<TeacherInfo>(`${(UsersService.SERVER_URL)}/teacher/${username}`);
	}
}
