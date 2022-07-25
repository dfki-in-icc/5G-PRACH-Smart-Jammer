
import { UntypedFormArray, FormControl, UntypedFormGroup } from '@angular/forms';
import { IArgType, IParam, IRow } from '../api/commands.api';
import { ParamCtrl } from './param.control';



enum RowFCN {
  paramsFA = 'params',
}


export class RowCtrl extends UntypedFormGroup {

  cmdName: string
  rawIndex: number

  constructor(row: IRow) {
    super({})

    this.cmdName = row.cmdName
    this.rawIndex = row.rawIndex
    this.addControl(RowFCN.paramsFA, new UntypedFormArray(row.params.map(param => new ParamCtrl(param))))
  }

  get paramsFA() {
    return this.get(RowFCN.paramsFA) as UntypedFormArray
  }

  set paramsFA(fa: UntypedFormArray) {
    this.setControl(RowFCN.paramsFA, fa);
  }

  get paramsCtrls(): ParamCtrl[] {
    return this.paramsFA.controls as ParamCtrl[]
  }

  api() {

    const doc: IRow = {
      rawIndex: this.rawIndex,
      cmdName: this.cmdName,
      params: this.paramsCtrls.map(control => control.api())
    }

    return doc

  }

}

