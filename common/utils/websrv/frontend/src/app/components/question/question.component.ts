/* eslint-disable @typescript-eslint/naming-convention */
import { Component, Inject } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormsModule } from '@angular/forms';
import { FormGroup, FormBuilder, Validators, NgForm } from '@angular/forms';
export interface QuestionDialogData {
   title: string;
  question: string;
  answer: string;
}
@Component({
  selector: 'app-question',
  templateUrl: './question.component.html',
  styleUrls: ['./question.component.css']
})
export class QuestionDialogComponent {

 constructor(
    public dialogRef: MatDialogRef<QuestionDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: QuestionDialogData,
  ) {}
  onNoClick() {
    this.dialogRef.close();
  }
}

