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

	securityQuestion: string;
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

export enum AuthType {
	OldPassword = "old-password",
	SecurityQuestion = "security-question"
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

	public signInBackup(credentials: Credentials) {
		return this.httpClient.post<UserInfo | null>(`${(UsersService.SERVER_URL)}/sign-in-backup`, credentials);
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

	public incrementSchoolYear(username: string) {
		return this.httpClient.put<StudentInfo>(`${(UsersService.SERVER_URL)}/student/${username}/increment`, undefined);
	}

	public updateProfileImage(username: string, profileImage: File) {
		const formData = new FormData();
		formData.append("profile-image", profileImage as Blob);
		return this.httpClient.post<UserInfo>(`${(UsersService.SERVER_URL)}/profile-image/${username}`, formData);
	}

	public getSecurityQuestion(username: string) {
		return this.httpClient.get(
			`${(UsersService.SERVER_URL)}/security-question/${username}`,
			{responseType: "text"}
		);
	}
	
	public isPasswordValid(password: string) {
		if (password === "")
			return "Нисте унијели лозинку.";

		if (password.length < 6 || password.length > 10)
			return "Лозинка мора имати између 6 и 10 карактера.";

		if (!/\d/.test(password))
			return "Лозинка мора садржати бар један број.";

		if (!/[A-Z]/.test(password))
			return "Лозинка мора садржати бар једно велико слово.";

		if (!/(.*[a-z]){3,}/.test(password))
			return "Лозинка мора садржати бар три мала слова.";

		if (!/[!@#$%^&*]/.test(password))
			return "Лозинка мора садржати бар један специјални карактер.";

		if (!/^[a-zA-Z]/.test(password))
			return "Лозинка мора почети словом.";

		return null;
	}

	public changePassword(username: string, newPassword: string, authAnswer: string, authType: AuthType) {
		const data = {
			newPassword,
			authAnswer,
			authType
		};

		return this.httpClient.put<UserInfo>(`${(UsersService.SERVER_URL)}/password/${username}`, data);
	}
}