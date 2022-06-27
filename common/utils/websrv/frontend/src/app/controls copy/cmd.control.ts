import { FormArray, FormControl, FormGroup } from '@angular/forms';
import { Subscription } from 'rxjs';
import { Observable } from 'rxjs/internal/Observable';
import { ICommand, ICommandOptions, IQuestion } from '../api/commands.api';

const enum CmdFCN {
  name = 'name',
  vars = 'variables',
  confirm = 'confirm',
  answer = "answer"
}

export class CmdCtrl extends FormGroup {

  confirm?: string
  question?: IQuestion
  cmdname: string
  options?: ICommandOptions[]
  public ResUpdTimer?: Observable<number>
  public ResUpdTimerSubscriber?: Subscription
  updbtnname: string
  constructor(cmd: ICommand) {
    super({});

    this.addControl(CmdFCN.name, new FormControl(cmd.name));
    this.addControl(CmdFCN.answer, new FormControl(""));
    this.addControl(CmdFCN.vars, new FormArray([]));


    this.confirm = cmd.confirm;
    this.question = cmd.question;
    this.cmdname = cmd.name;
    this.options = cmd.options;
    this.updbtnname = "Start update"
  }

  api() {
    const doc: ICommand = {
      name: this.nameFC.value,
      param: this.question
        ? { name: this.question!.pname, value: this.answerFC.value, type: this.question!.type, modifiable: false }
        : undefined,
      options: this.options
    };

    return doc;
  }

  modulename() {
    return this.cmdname;
  }

  isResUpdatable(): boolean {
    if (this.options) {
      for (let opt = 0; opt < this.options.length; opt++) {
        if (this.options[opt] == ICommandOptions.update)
          return true;
      }
    } else {
      return false;
    }
    return false;
  }

  stopUpdate() {
    if (this.ResUpdTimerSubscriber) {
      this.ResUpdTimerSubscriber.unsubscribe();
      this.updbtnname = "Start update"
    }
  }

  startUpdate() {
    if (this.ResUpdTimerSubscriber && this.ResUpdTimer) {
      this.ResUpdTimerSubscriber = this.ResUpdTimer.subscribe();
      this.updbtnname = "Stop update"
    }
  }

  get nameFC() {
    return this.get(CmdFCN.name) as FormControl;
  }

  set nameFC(fc: FormControl) {
    this.setControl(CmdFCN.name, fc);
  }

  get answerFC() {
    return this.get(CmdFCN.answer) as FormControl;
  }

  get varsFA() {
    return this.get(CmdFCN.vars) as FormArray;
  }

  set varsFA(fa: FormArray) {
    this.setControl(CmdFCN.vars, fa);
  }
}

