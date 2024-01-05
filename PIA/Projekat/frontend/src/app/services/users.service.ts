import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";

export interface Credentials {
	username: string;
	password: string;
}

export enum Role {
	Student,
	Teacher
}

export interface UserInfo {
	role? : Role;

	firstName: string;
	lastName: string;
}

export interface StudentInfo {
	credentials: Credentials;
	info: UserInfo;
}

export interface TeacherInfo {
	credentials: Credentials;
	info: UserInfo;
}

@Injectable({
	providedIn: "root"
})
export class UsersService {
	public constructor(private readonly httpClient: HttpClient) {

	}

	public signIn(credentials: Credentials) {
		return this.httpClient.post<UserInfo | null>("http://localhost:3000/users/sign-in", credentials);
	}

	public signUpStudent(studentInfo: StudentInfo) {
		return this.httpClient.post<UserInfo>("http://localhost:3000/users/sign-up/student", studentInfo);
	}

	public signUpTeacher(teacherInfo: TeacherInfo) {
		return this.httpClient.post<UserInfo>("http://localhost:3000/users/sign-up/teacher", teacherInfo);
	}
}
