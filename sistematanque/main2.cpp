#include <iostream>

extern void AbrirValvula();
extern void CerrarValvula();

extern void EncenderBomba();
extern void ApagarBomba();

extern bool SVTanque;
extern bool SLTanque;
extern bool SVCisterna;
extern bool SLCisterna;

enum ESTADO
{
    VACIO = 0,
    LLENO,
    DESCARGANDO,
    CARGANDO
};

ESTADO obtenerEstadoTanque(ESTADO anterior)
{

    if(SVTanque == true && anterior == VACIO)
        return VACIO;
    if(SLTanque == true && anterior == LLENO)
        return LLENO;
    if(SLTanque != true && SVTanque != true && anterior == LLENO)
        return DESCARGANDO;
    if(SLTanque != true && SVTanque != true && anterior == VACIO)
        return CARGANDO;

}

ESTADO obtenerEstadoCisterna(ESTADO anterior)
{
    if(SVCisterna == true && anterior == VACIO)
        return VACIO;
    if(SLCisterna == true && anterior == LLENO)
        return LLENO;
    if(SLCisterna != true && SVCisterna != true && anterior == LLENO)
        return DESCARGANDO;
    if(SLCisterna != true && SVCisterna != true && anterior == VACIO)
        return CARGANDO;
}


int main()
{
    ESTADO estadoActualTanque;
    ESTADO estadoAnteriorTanque;

    ESTADO estadoActualCisterna;
    ESTADO estadoAnteriorCisterna;

    while(1)
    {
        estadoActualCisterna = obtenerEstado(estadoAnteriorCisterna);
        estadoActualTanque = obtenerEstado(estadoAnteriorTanque);

        if(estadoActualTanque == VACIO)
        {
            if(estadoActualCisterna == VACIO)
            {
                AbrirValvula();
            }
            else if(estadoActualCisterna == LLENO)
            {
                CerrarValvula();
            }

            EncenderBomba();
        }
        else if(estadoActualTanque == LLENO)
        {
            ApagarBomba();
        }
        else if(estadoActualTanque == CARGANDO)
        {
            if(estadoActualCisterna == CARGANDO)
            {
                AbrirValvula();
            }

            EncenderBomba();
        }
        else if(estadoActualTanque == DESCARGANDO)
        {
            ApagarBomba();
        }
        else if(estadoActualTanque == VACIO)
        {

            if(estadoActualCisterna == VACIO)
            {
                while(SLCisterna == false)
                    AbrirValvula();
            }

            EncenderBomba();
        }

        estadoAnterior = estadoActual;
    }
}
