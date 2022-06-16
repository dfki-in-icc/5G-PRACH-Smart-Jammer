import { Component } from '@angular/core';
import { Observable } from 'rxjs/internal/Observable';
import { of } from 'rxjs/internal/observable/of';
import { map } from 'rxjs/internal/operators/map';
import { mergeMap } from 'rxjs/internal/operators/mergeMap';
import { filter } from 'rxjs/operators';
import { CommandsApi, IVariable, IArgType, IColumn, ICommand, IParam, ICommandOptions } from 'src/app/api/commands.api';
import { CmdCtrl } from 'src/app/controls/cmd.control';
import { ParamFC } from 'src/app/controls/param.control';
import { RowCtrl } from 'src/app/controls/row.control';
import { VarCtrl } from 'src/app/controls/var.control';
import { DialogService } from 'src/app/services/dialog.service';
import { LoadingService } from 'src/app/services/loading.service';
import { forkJoin } from 'rxjs';
import { DownloadService } from 'src/app/services/download.service';

@Component({
  selector: 'app-commands',
  templateUrl: './commands.component.html',
  styleUrls: ['./commands.component.css'],
})
export class CommandsComponent {

  IArgType = IArgType;

  vars$: Observable<VarCtrl[]>
  modules$: Observable<CmdCtrl[]>
  modulevars$?: Observable<VarCtrl[]>
  modulecmds$?: Observable<CmdCtrl[]>

  selectedModule?: CmdCtrl
  selectedCmd?: ICommand

  //table columns
  displayedColumns: string[] = []
  rows$: Observable<RowCtrl[]> = new Observable<RowCtrl[]>()
  columns: IColumn[] = []

  constructor(
    public commandsApi: CommandsApi,
    public loadingService: LoadingService,
    public dialogService: DialogService,
    public downloadService: DownloadService,
    
  ) {
    this.vars$ = this.commandsApi.readVariables$().pipe(
      map((vars) => vars.map(ivar => new VarCtrl(ivar)))
    );

    this.modules$ = this.commandsApi.readCommands$().pipe(
      map((cmds) => cmds.map(icmd => new CmdCtrl(icmd, this.commandsApi)))
    );
  }


  onModuleSelect(control: CmdCtrl) {

    this.selectedModule = control
    this.selectedCmd = undefined

    this.modulecmds$ = this.commandsApi.readCommands$(`${control.nameFC.value}`).pipe(
      map(icmds => icmds.map(icmd => new CmdCtrl(icmd, this.commandsApi)))
    )

    this.modulevars$ = this.commandsApi.readVariables$(`${control.nameFC.value}`).pipe(
      map(ivars => ivars.map(ivar => new VarCtrl(ivar))),
    )
  }

  onVarSubmit(control: VarCtrl) {
	let selectedVar:IVariable = control.api();
	if (selectedVar.type != IArgType.configfile)
      this.commandsApi.setVariable$(selectedVar,"").subscribe();
	if (selectedVar.type == IArgType.configfile) {
      this.downloadService.getFile(selectedVar.value)
    }     
  }

  onModuleVarsubmit(control: VarCtrl) {
    this.commandsApi.setVariable$(control.api(), `${this.selectedModule!.nameFC.value}`)
      .pipe(
        map(resp => this.dialogService.openVarRespDialog(resp))
      ).subscribe();
  }

  execCmd(control: CmdCtrl) {
      const obs= this.commandsApi.runCommand$(control!.api(), control!.question ? this.selectedModule!.modulename() : `${this.selectedModule!.nameFC.value}`)
 //     this.rows$ = obs.pipe(
        obs.subscribe(
        resp => {
          if (resp.display[0])  this.dialogService.openCmdDialog(control,resp, 'cmd ' + control.nameFC.value + ' response:')
//          else return of(resp)
          var controls: RowCtrl[] = []
          this.displayedColumns = []

          if (resp.table) {
            this.columns = resp.table.columns
            this.displayedColumns = this.columns.map(col => col.name)
            this.displayedColumns.push('button')

            for (let rawIndex = 0; rawIndex < resp.table.rows.length; rawIndex++) {

              let params: IParam[] = []
              for (let i = 0; i < this.columns.length; i = i + 1) {
                params.push({
                  value: resp.table.rows[rawIndex][i],
                  col: this.columns[i]
                })
              }

              controls[rawIndex] = new RowCtrl({
                params: params,
                rawIndex: rawIndex,
                cmdName: this.selectedCmd!.name
              })
            }
          }
          this.rows$=of(controls)
          return this.rows$
        },
          err => console.error('execCmd error: ' + err),
          () => console.log('execCmd completed: '),
        ) // map resp
        

    return of(null);	  
  }
  
  onCmdSubmit(control: CmdCtrl) {

    this.selectedCmd = control.api()
    const obsparam = forkJoin ([control.confirm
      ? this.dialogService.openConfirmDialog(control.confirm)
      : control.question
      ? this.dialogService.openQuestionDialog(this.selectedModule!.modulename() + " " + control.modulename(), control)
      : of(true)]);
    obsparam.subscribe ( results => {
	  if ( !results[0] )
	    return of(null);
    return(this.execCmd(control));
  }); // subscribe obsparam results
  return of(null);
}

  onParamSubmit(control: RowCtrl) {
    this.commandsApi.setRow$(control.api(), this.selectedModule?.nameFC.value).subscribe();
  }

  isRowModifiable(control: RowCtrl) {
    const modif = control.paramsFA.controls
      .map(paramFC => (paramFC as ParamFC).api().col!.modifiable)
      .reduce((acc, next) => acc || next)

    return modif
  }


}
