
import { FormArray, FormControl, FormGroup } from '@angular/forms';
import { IArgType, IParam, IRow } from '../api/commands.api';
import { ParamCtrl } from './param.control';



enum RowFCN {
  paramsFA = 'params',
}


export class RowCtrl extends FormGroup {

  cmdName: string
  rawIndex: number

  constructor(row: IRow) {
    super({})

    this.cmdName = row.cmdName
    this.rawIndex = row.rawIndex

    this.addControl(RowFCN.paramsFA, new FormArray(row.params.map(param => {
      let control: FormControl
      switch (param.col.type) {
        case IArgType.boolean:
          control = new FormControl((param.value === 'true') ? true : false);
          break;

        case IArgType.loglvl:
          control = new FormControl(param.value);
          break;

        default:
          control = new FormControl(param.value)
      }

      if (!param.col.modifiable) control.disable()

      return control

    })))

  }

  constructor(row: IRow) {
    super({})

    const iparams: IParam[] = this.row.params.map(param => {
      switch (param.col?.type) {
        case IArgType.boolean:
          return this.controls[this.row.rawIndex].value as string;

        default:
          return this.controls[this.row.rawIndex].value
      }
    })

  set paramsFA(fa: FormArray) {
    this.setControl(RowFCN.paramsFA, fa);
  }

  get paramsCtrls(): ParamCtrl[] {
    return this.paramsFA.controls as ParamCtrl[]
  }

  addParamCtrl = (control: ParamCtrl) => {
    this.paramsFA.push(control)

    return this
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

