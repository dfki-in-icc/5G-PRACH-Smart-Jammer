import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment } from 'src/environments/environment';
import { route } from 'src/app/api/commands.api';

export enum IScopeGraphType {
    IQs = "IQs",
    LLR = "LLR",
    UNSUP = "UNSUP"
}

export interface IGraphDesc {
    title: string;
    type: IScopeGraphType;
    id:   number;
}

export interface IScopeDesc {
    title: string;
    graphs: IGraphDesc[];
}

export interface IScopeCmd {
    name: string;
    graphid?: number;
    value:   string;
}

@Injectable({
    providedIn: 'root',
})
export class ScopeApi {
    constructor(private httpClient: HttpClient) { }

    public getScopeInfos$ = () => this.httpClient.get<IScopeDesc>(environment.backend + route + 'scopectrl/');

    public setScopeParams$ = (cmd: IScopeCmd) => this.httpClient.post(environment.backend + route + 'scopectrl/', cmd);

}
