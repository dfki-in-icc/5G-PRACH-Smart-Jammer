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
  question?: IQuestion
  cmdname:   string
  constructor(cmd: ICommand) {
    super({});

    this.addControl(CmdFCN.name, new FormControl(cmd.name));
    this.confirm = cmd.confirm;
    this.addControl(CmdFCN.answer, new FormControl(""));
    this.question = cmd.question;
    this.cmdname = cmd.name;
  }

  api() {
    const doc: ICommand = {
      name: this.nameFC.value,
      param: {name:"",value:this.answerFC.value, type: this.question?this.question?.type:IArgType.string, modifiable:false}
    };

    return doc;
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

