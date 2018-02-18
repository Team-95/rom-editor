export enum UIMessageType {
    success,
    info,
    warn,
    error
}

export class UIMessage {
    
    message: String;
    type: UIMessageType;

    constructor(message: String, type: UIMessageType) {
        this.message = message;
        this.type = type;
    }

}

