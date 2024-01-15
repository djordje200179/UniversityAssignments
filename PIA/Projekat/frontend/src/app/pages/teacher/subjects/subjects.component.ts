import {Component, Input, OnInit} from "@angular/core";
import {CommonModule} from "@angular/common";
import {MatCardModule} from "@angular/material/card";
import {TeachersService} from "../../../services/teachers.service";
import {MatListModule} from "@angular/material/list";
import {MatLineModule} from "@angular/material/core";

@Component({
	selector: "app-subjects",
	standalone: true,
	imports: [CommonModule, MatCardModule, MatListModule, MatLineModule],
	templateUrl: "./subjects.component.html",
	styleUrls: ["./subjects.component.scss"]
})
export class SubjectsComponent implements OnInit {
	@Input()
	public teacherUsername!: string;

	public subjects? : string[];

	public constructor(private readonly teachersService: TeachersService) {

	}

	public ngOnInit(): void {
		this.teachersService.getTeacherEnrollments(this.teacherUsername).subscribe(
			subjects => this.subjects = subjects.map(enrollment => enrollment.subject),
			console.error
		);
	};
}
