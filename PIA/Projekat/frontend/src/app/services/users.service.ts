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
	role : Role;

	firstName: string;
	lastName: string;
	gender: Gender;

	securityQuestion: string;
	securityAnswer?: string;

	address: string;
	phoneNumber: string;
	email: string;

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
	credentials: Credentials;
	info: UserInfo;

	biography?: File;
	subjects: string[];
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
		console.log(teacherInfo);
		return this.httpClient.post<TeacherInfo>(`${(UsersService.SERVER_URL)}/sign-up/teacher`, teacherInfo);
	}
}