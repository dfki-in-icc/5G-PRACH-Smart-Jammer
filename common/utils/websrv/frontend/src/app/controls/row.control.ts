
import { FormArray, FormControl } from '@angular/forms';
import { IArgType, IParam, IRow } from '../api/commands.api';


export class RowsFA extends FormArray {

  constructor(public row: IRow) {
    super([])

    row.params.map(param => {
      switch (param.col?.type) {
        case IArgType.boolean:
          this.controls[row.rawIndex] = new FormControl((param.value === 'true') ? true : false);
          break;

        case IArgType.loglvl:
          this.controls[row.rawIndex] = new FormControl(param.value);
          break;

        default:
          this.controls[row.rawIndex] = new FormControl(param.value)
      }
    })
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

    const doc: IRow = {
      rawIndex: this.row.rawIndex,
      cmdName: this.row.cmdName,
      params: iparams
    }

    return doc

  }

}

