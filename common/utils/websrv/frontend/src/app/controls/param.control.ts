import { FormControl } from '@angular/forms';
import { IParam } from '../api/commands.api';


export class ParamFC extends FormControl {

  constructor(
    public param: IParam
  ) {
    super(param.value);
  }

  api() {
    const doc: IParam = {
      value: this.value,
      col: this.param.col
    };

    return doc;
  }
}

