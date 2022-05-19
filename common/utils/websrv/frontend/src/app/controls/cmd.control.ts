import { FormControl, FormGroup } from '@angular/forms';
import { ICommand } from '../api/commands.api';
import { IQuestion } from '../api/commands.api';
import { IArgType } from '../api/commands.api';

const enum CmdFCN {
  name = 'name',
  confirm = 'confirm',
  answer = "answer"
}

export class CmdCtrl extends FormGroup {

  confirm?: string
  question?:IQuestion
  cmdname: string
  constructor(cmd: ICommand) {
    super({});

    this.addControl(CmdFCN.name, new FormControl(cmd.name));
    this.addControl(CmdFCN.answer, new FormControl(""));
    this.confirm = cmd.confirm;
    this.question = cmd.question;
    this.cmdname = cmd.name;
  }

  api() {
    const doc: ICommand = {
      name: this.nameFC.value,
      param: this.question
        ? { name: this.question!.pname, value: this.answerFC.value, type: this.question!.type , modifiable: false }
        : undefined
    };

    return doc;
  }

  modulename() {
    return this.cmdname;
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


}

