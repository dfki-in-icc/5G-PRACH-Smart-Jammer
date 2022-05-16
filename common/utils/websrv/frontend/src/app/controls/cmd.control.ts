import { FormControl, FormGroup } from '@angular/forms';
import { ICommand } from '../api/commands.api';
import { IQuestion } from '../api/commands.api';

const enum CmdFCN {
  name = 'name',
  confirm = 'confirm'
}

export class CmdCtrl extends FormGroup {

  confirm?: string
  question?: IQuestion
  constructor(cmd: ICommand) {
    super({});

    this.addControl(CmdFCN.name, new FormControl(cmd.name));
    this.confirm = cmd.confirm
    this.question = cmd.question
  }

  api() {
    const doc: ICommand = {
      name: this.nameFC.value
    };

    return doc;
  }

  get nameFC() {
    return this.get(CmdFCN.name) as FormControl;
  }

  set nameFC(fc: FormControl) {
    this.setControl(CmdFCN.name, fc);
  }
}

