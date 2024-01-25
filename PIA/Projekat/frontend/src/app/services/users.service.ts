import {Injectable} from "@angular/core";
import {HttpClient} from "@angular/common/http";

export interface Credentials {
	username: string;
	password: string;
}

export enum Role {
	Student = "student",
	Teacher = "teacher",
	Admin = "admin"
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

	activated?: boolean;
	blocked?: boolean;
}


export interface Notification {
	receiverUsername: string;
	message: string;
	createdAt: Date;
	seen: boolean;
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

	public signInAdmin() {
		return this.httpClient.post<UserInfo | null>(
			"http://localhost:8080/users/sign-in-admin",
			undefined,
			{withCredentials: true}
		);
	}

	public signUpStudent(studentInfo: StudentInfo) {
		const formData = new FormData();
		if (studentInfo.info.profileImage)
			formData.append("profile-image", studentInfo.info.profileImage as Blob);

		formData.append("data", JSON.stringify(studentInfo));

		return this.httpClient.post<StudentInfo>(`${(UsersService.SERVER_URL)}/sign-up/student`, formData);
	}

	public signUpTeacher(teacherInfo: TeacherInfo) {
		const formData = new FormData();
		if (teacherInfo.info.profileImage)
			formData.append("profile-image", teacherInfo.info.profileImage as Blob);

		formData.append("biography", teacherInfo.biography as Blob);

		formData.append("data", JSON.stringify(teacherInfo));

		return this.httpClient.post<TeacherInfo>(`${(UsersService.SERVER_URL)}/sign-up/teacher`, formData);
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

	public signOut() {
		localStorage.removeItem("user-info");
	}

	public getStudentInfo(username: string) {
		return this.httpClient.get<StudentInfo>(`${(UsersService.SERVER_URL)}/student/${username}`);
	}

	public getTeacherInfo(username: string) {
		return this.httpClient.get<TeacherInfo>(`${(UsersService.SERVER_URL)}/teacher/${username}`);
	}

	public getProfileImageURL(username: string) {
		return `${(UsersService.SERVER_URL)}/profile-image/${username}`;
	}

	public getAllTeachers() {
		return this.httpClient.get<TeacherInfo[]>(`${(UsersService.SERVER_URL)}/teachers`);
	}

	public getAllStudents() {
		return this.httpClient.get<StudentInfo[]>(`${(UsersService.SERVER_URL)}/students`);
	}

	public getAllTeacherRequests() {
		return this.httpClient.get<TeacherInfo[]>(`${(UsersService.SERVER_URL)}/teachers/requests`);
	}

	public getNotifications(username: string) {
		return this.httpClient.get<Notification[]>(`${(UsersService.SERVER_URL)}/notifications/${username}`);
	}
}