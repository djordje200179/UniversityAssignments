import {NgModule} from "@angular/core";
import {RouterModule, Routes} from "@angular/router";
import {WelcomeComponent} from "./pages/welcome/welcome.component";
import {AccountComponent} from "./pages/account/account.component";
import {TeachersComponent} from "./pages/teachers/teachers.component";
import {TeacherComponent} from "./pages/teacher/teacher.component";
import {ClassesComponent} from "./pages/classes/classes.component";
import {StudentsComponent} from "./pages/students/students.component";
import {StudentComponent} from "./pages/student/student.component";

const routes: Routes = [
	{
		path: "",
		component: WelcomeComponent
	},
	{
		path: "account",
		component: AccountComponent
	},
	{
		path: "teachers",
		component: TeachersComponent
	},
	{
		path: "teacher/:username",
		component: TeacherComponent
	},
	{
		path: "classes",
		component: ClassesComponent
	},
	{
		path: "students",
		component: StudentsComponent
	},
	{
		path: "student/:username",
		component: StudentComponent
	}
];

@NgModule({
	imports: [RouterModule.forRoot(routes)],
	exports: [RouterModule]
})
export class AppRoutingModule {
}
