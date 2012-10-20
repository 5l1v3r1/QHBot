#include "qhbotcommands.h"

QHBotCommands::QHBotCommands(QHBotUserManager* UserManager, QObject *parent): QObject(parent)
{
    this->UserManager=UserManager;
    this->commands<<"hello"<<"invite"<<"setnick"<<"snooze"<<"list";

    quotes<<"Wow, tu no est�s muerto?";
    quotes<<"Yoo hoooooooooo!";
    quotes<<"I am the best robot. Yeah, yeah, yeah, I am the best robot. Ooh, ooh, here we go!";
    quotes<<"O dios m�o, estoy goteando! Creo que estoy goteando! Ahhh, hay aceite por todas partes!";
    quotes<<"Puedo ver a trav�s del tiempo...";
    quotes<<"Como nuevo, creo. Estoy goteando?";
    quotes<<"Oh, mira, es otra bolsa de carne maloliente";
    quotes<<"Tengo dos opciones. Puedo escucharos mover vuestra repugnante carne... �... puedo enchufar un electrodo en mi panel trasero y llamarlo para�so";
    quotes<<"Estoy muy contento de no estar hecho de carne sabrosa como vosotros";
    quotes<<"Que nadie se mueva! He perdido mi lentilla...";
    quotes<<"He ganado los siguientes premios este a�o: 'Claptrap m�s eficaz en situaciones potencialmente mortales', 'Bailar�n de Breackdance m�s caliente', 'Orador maestro' y 'Mejor beso'";
    quotes<<"Dood doo doo";
    quotes<<"Matar� a la persona que querais si alguien me deja salir de aqu�. Nunca me gust� Snifer, es solo una sugerencia";
    quotes<<"No me d�is la espalda si no quer�is no poder dar la espalda a nada jam�s. Eso son� a amenaza... puedo volver a intentarlo?";
    quotes<<"Vale, ahora estoy mentalmente sano. No m�s maldades, ahora soy Robo-Teresa. DEJADME SALIR!!";
    quotes<<"No me ames... extra�o. He sido lastimado demasiadas veces... por zombies";
    quotes<<"Ha pasado mucho desde que llegue aqu�. Ahora estoy como en casa... excepto por los zombies";
    quotes<<"V al aserradero, dijeron. Va a ser bueno para su carrera, dijeron. AYUDA!";
    quotes<<"�Ayudadme! �Hay alguien ah�?";
    quotes<<"Me vendr�a bien un .. pbht pbht .. Bien! Ahora tengo un .. pbht pbht ... aserr�n en mi unidad de enunciaci�n vocal";
    quotes<<"�Hay alguna persona de carne y hueso por ah�?";
    quotes<<"Vamos, sac�dme de aqu�! Ella no me dijo su edad!";
    quotes<<"Hey, mirad. Puedo hacer lo que sea por salir de aqu�. Pensad en ello. Nada es demasiado grande, nada es demasiado peque�o, no s� si lo pill�is...";
    quotes<<"No estoy sujeto a vuestras leyes. No me programaron para hacer esto!";
    quotes<<"Har� lo que quer�is para salir de aqui. Hablando en serio, lo digo en serio. Cualquier cosa que querais. Cualquier cosa. Pensad en ello";
    quotes<<"Cuando salga de aqui, putos, algunos de vosotros sereis apu�alados. De hecho, batir� el record de apu�alamientos en este planta.";
    quotes<<"Aaah! �Oh, no! AAAAHHHHH!";
    quotes<<"Alguien conoce a un exorcista? No, nada? Ok";
}

bool QHBotCommands::isCommand(const QXmppMessage &msg)
{
    if(msg.body().startsWith("$") || msg.body().startsWith("/") ) return true;
    else return false;
}

void QHBotCommands::runCommand(const QXmppMessage &msg)
{
    qDebug()<<"Run command!!";

    QStringList arg=msg.body().split(" ");
    QString CommandName = arg.at(0);
    QString from = msg.from();
    //FIXME: FIX PERMISOS
    QString fromEmail = from.split("/").at(0);
    CommandName.remove(0,1);

    arg.removeAt(0);

    switch(commands.indexOf(CommandName,0))
    {
    case -1://Si no encuentra el comando.
        qWarning()<<"Comando " + CommandName+ " No Encontrado";
        break;

    case 0://Si recibe hello
        qDebug()<< "Ejecutando commando "+CommandName;
        this->runCmdHello(arg);
        break;

    case 1://Si recibe invite
        //FIXME: FIX PERMISOS
        if(!admList.contains(fromEmail)) return;
        qDebug()<< "Ejecutando commando "+CommandName;
        this->runCmdInvite(arg);
        break;

    case 2://Si recibe setnick
        qDebug()<< "Ejecutando commando "+CommandName;
        this->runCmdSetNick(arg, fromEmail);
        break;

    case 3:
        qDebug()<< "Ejecutando commando "+CommandName;
        this->runCmdSnoozing(arg,from);
        break;

    case 4:
        qDebug()<<"Ejecutando comando "+CommandName;
        this->runCmdList(arg,from);

    }
}

void QHBotCommands::runCmdHello(const QStringList &arg)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    emit messageRequest(QXmppMessage("bot@h-sec.org","broadcast",quotes.at((qrand()%(28)))));
}

void QHBotCommands::runCmdInvite(const QStringList &arg)
{
    QRegExp rx("^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}$");
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    if(arg.length() < 1) return;
    if(!rx.exactMatch(arg.at(0))) return;
    if(UserManager->inviteUser(arg.at(0)))
    {
        qDebug()<<arg.at(0)<<" ha sido invitado";
        emit messageRequest(QXmppMessage("bot@h-sec.org","broadcast",arg.at(0)+" ha sido invitado"));
    }
}
void QHBotCommands::runCmdSetNick(const QStringList &arg, const QString &from)
{
    if(arg.length() < 2 || UserManager->getUser(arg.at(0)) == NULL) //No esta en la lista? No hacemos nada.
        return;

    QString jid=arg.at(0); //Si el FIX PERMISOS se elimina, esto podría ser const.

    /* FIXME: FIX PERMISOS */
    if(!admList.contains(from)){
        jid = from;
    }
    /* END FIXME */

    const QString& newNick=arg.at(1);


    UserManager->getUser(jid)->setNick(newNick);

    emit messageRequest(QXmppMessage("bot@h-sec.org","broadcast",jid+" es ahora conocido como "+newNick));
}

void QHBotCommands::runCmdSnoozing(const QStringList &arg,const QString &from)
{
    if(arg.length() > 0 && (arg.at(0)=="on" || arg.at(0)=="off")){
        UserManager->getUser(from.split("/").at(0))->setSnooze(arg.at(0)=="on");
        emit messageRequest(QXmppMessage("", from, "Modo snooze "+arg.at(0)));
    }
}

void QHBotCommands::runCmdList(const QStringList &arg, const QString &from)
{
    QStringList UserList;
    foreach(QHBotUser* u,UserManager->getUsers())
    {
        UserList.append(QString("["+u->getNick()+"] ("+u->getJID()+")"));
        UserList.last().insert(0,u->isAvailable()?"[+]":"[-]");
    }

    UserList.sort();

    emit messageRequest(QXmppMessage("",from,"\n"+UserList.join("\n")));
}

// FIXME: FIX PERMISOS
void QHBotCommands::setAdmList(QStringList admList)
{
    this->admList = admList;
}
