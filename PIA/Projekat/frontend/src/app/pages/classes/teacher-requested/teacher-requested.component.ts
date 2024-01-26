import {Component, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatButtonModule} from "@angular/material/button";
import {MatButtonToggleChange, MatButtonToggleModule} from "@angular/material/button-toggle";
import {MatTableModule} from "@angular/material/table";
import {ClassesService, ClassInfo} from "../../../services/classes.service";
import {UsersService} from "../../../services/users.service";
import {MatIconModule} from "@angular/material/icon";
import {MatSortModule} from "@angular/material/sort";

interface Row {
	id: number;
	studentName: string;
	topic: string;
	subject: string;
	rating: number;
	time: Date;
}

@Component({
	selector: "app-teacher-requested",
	standalone: true,
	imports: [CommonModule, MatButtonModule, MatButtonToggleModule, MatTableModule, MatIconModule, MatSortModule],
	templateUrl: "./teacher-requested.component.html",
	styleUrls: ["./teacher-requested.component.scss"]
})
export class TeacherRequestedComponent implements OnInit {
	public tableData?: Row[];

	public displayedColumns: string[] = ["topic", "subject", "studentName", "rating", "time", "actions"];

	public constructor(private readonly usersService: UsersService, private readonly classesService: ClassesService) {

	}

	public ngOnInit(): void {
		const teacherUsername = this.usersService.getCurrentUser()!.username!;

		this.classesService.getRequested(teacherUsername).subscribe(
			classes=> {
				for (const c of classes)
					c.time = new Date(c.time);

				classes.sort((a, b) => b.time.getTime() - a.time.getTime());

				const tableData: Row[] = [];
				for (const c of classes) {
					const studentInfo = c.student.info;
					const studentName = `${studentInfo.firstName} ${studentInfo.lastName}`;

					const studentRow = {
						id: c.id,
						studentName: studentName,
						topic: c.topic,
						subject: c.subject,
						time: c.time,
						rating: -1
					};

					tableData.push(studentRow);

					this.classesService.getStudentRating(c.student.info.username!).subscribe(
						rating => {
							studentRow.rating = parseInt(rating);
						},
						console.error
					);
				}

				this.tableData = tableData;
			},
			console.error
		);
	}

	public acceptClass(id: number): void {
		this.classesService.acceptClass(id).subscribe(
			() => {
				alert("Успјешно сте прихватили час!");
				window.location.reload();
			}
		);
	}

	public rejectClass(id: number): void {
		const reason = prompt("Који је разлог одбијања часа?");
		if (reason === null)
			return;

		this.classesService.cancelClass(id, reason).subscribe(
			() => {
				alert("Успјешно сте отказали час!");
				window.location.reload();
			}
		);
	}
}
