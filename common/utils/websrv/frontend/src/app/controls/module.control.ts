import { FormArray, FormGroup } from '@angular/forms';
import { IModule } from '../api/commands.api';

const enum ModuleFCN {
  vars = 'variables',
  cmds = 'commands'
}

export class ModuleCtrl extends FormGroup {

  name: string

  constructor(imodule: IModule) {
    super({})
    this.name = imodule.name
    this.addControl(ModuleFCN.vars, new FormArray([]))
    this.addControl(ModuleFCN.cmds, new FormArray([]))
  }

  get varsFA() {
    return this.get(ModuleFCN.vars) as FormArray;
  }

  set varsFA(fa: FormArray) {
    this.setControl(ModuleFCN.vars, fa);
  }

  get cmdsFA() {
    return this.get(ModuleFCN.cmds) as FormArray;
  }

  set cmdsFA(fa: FormArray) {
    this.setControl(ModuleFCN.cmds, fa);
  }


}

