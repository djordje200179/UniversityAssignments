import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {TeacherInfo, UsersService} from "../../services/users.service";
import {ActivatedRoute} from "@angular/router";
import {ClassesService} from "../../services/classes.service";
import {MatButtonModule} from "@angular/material/button";
import {MatIconModule} from "@angular/material/icon";
import {MatTableModule} from "@angular/material/table";
import {CommentComponent, Rating} from "../../components/comment/comment.component";
import {MAT_DIALOG_DEFAULT_OPTIONS, MatDialog, MatDialogModule} from "@angular/material/dialog";

interface Row {
	id: number;
	topic: string;
	subject: string;
	time: Date;
	teacherComment: string | null;
	teacherRating: number | null;
}

@Component({
	selector: "app-student",
	standalone: true,
	imports: [
		CommonModule, MatButtonModule,
		MatIconModule, MatTableModule
	],
	templateUrl: "./student.component.html",
	styleUrls: ["./student.component.scss"],
})
export class StudentComponent implements OnInit {
	public readonly teacherUsername: string;
	public readonly studentUsername: string;

	public tableData?: Row[];

	public displayedColumns: string[] = ["topic", "subject", "time", "teacherComment", "teacherRating"];

	public constructor(
		private readonly activatedRoute: ActivatedRoute,
		private readonly usersService: UsersService,
		private readonly classesService: ClassesService,
		public dialog: MatDialog
	) {
		this.studentUsername = this.activatedRoute.snapshot.params["username"];
		this.teacherUsername = this.usersService.getCurrentUser()!.username!;
	}

	public ngOnInit(): void {
		this.classesService.getAllTeacherStudentClasses(
			this.teacherUsername, this.studentUsername
		).subscribe(
			classes =>{
				for (const c of classes)
					c.time = new Date(c.time);

				classes.sort((a, b) => b.time.getTime() - a.time.getTime());

				const tableData: Row[] = [];
				for (const c of classes) {
					const studentInfo = c.student.info;
					const studentName = `${studentInfo.firstName} ${studentInfo.lastName}`;

					const studentRow = {
						id: c.id,
						topic: c.topic,
						subject: c.subject,
						time: c.time,
						teacherComment: c.teacherComment,
						teacherRating: c.teacherRating
					};

					tableData.push(studentRow);
				}

				this.tableData = tableData;
			},
			console.error
		);
	}

	public commentClass(id: number): void {
		const dialogRef = this.dialog.open<CommentComponent, any, Rating>(
			CommentComponent,
			{
				height: '370px',
				width: '600px',
			}
			);

		dialogRef.afterClosed().subscribe(
			result => {
				if (!result)
					return;

				if (!result.comment || !result.rating) {
					alert("Нисте унијели све потребне податке.");
					return;
				}

				this.classesService.commentClass(id, true, result.comment, result.rating).subscribe(
					() => {
						alert("Успјешно сте оцјенили час.");
						const c = this.tableData!.find(c => c.id === id)!;
						c.teacherComment = result.comment;
						c.teacherRating = result.rating;

						this.tableData = this.tableData!;
					},
					console.error
				);
			}
		);
	}
}
