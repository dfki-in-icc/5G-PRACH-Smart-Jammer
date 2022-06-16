import { FormControl, FormGroup } from '@angular/forms';
import { CommandsApi, IArgType, ICommand, IQuestion, ICommandOptions } from 'src/app/api/commands.api';
const enum CmdFCN {
  name = 'name',
  confirm = 'confirm',
  answer = "answer"
}

export class CmdCtrl extends FormGroup {

  confirm?: string
  question?:IQuestion
  cmdname: string
  options?: ICommandOptions[]
  commandsApi: CommandsApi
  constructor(cmd: ICommand, commandsApi: CommandsApi) {
    super({});

    this.addControl(CmdFCN.name, new FormControl(cmd.name));
    this.addControl(CmdFCN.answer, new FormControl(""));
    this.confirm = cmd.confirm;
    this.question = cmd.question;
    this.cmdname = cmd.name;
    this.options = cmd.options;
    this.commandsApi = commandsApi;
  }

  api() {
    const doc: ICommand = {
      name: this.nameFC.value,
      param: this.question
        ? { name: this.question!.pname, value: this.answerFC.value, type: this.question!.type , modifiable: false }
        : undefined,
      options: this.options
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

