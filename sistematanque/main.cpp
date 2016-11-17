void abrirValvula();
void cerrarValvula();

void EncenderBomba();
void ApagarBomba();

bool sensorLlenoTanque();
bool sensorVacioTanque();

bool sensorLlenoCisterna();
bool sensorVacioCisterna();

int main(int argc, char* argv[])
{
    while(true)
    {
        if(sensorVacioTanque())
        {
            if(sensorVacioCisterna())
            {
                while(!sensorLlenoCisterna())
                    abrirValvula();

                cerrarValvula();

                while(!sensorLlenoTanque())
                    EncenderBomba();

                ApagarBomba();
            }
            else if(sensorLlenoCisterna())
            {
                    while(!sensorLlenoTanque())
                        EncenderBomba();

                    ApagarBomba();
            }
        }
        else if(sensorLlenoTanque())
            ApagarBomba();

        if(sensorVacioCisterna())
        {
        while(!sensorLlenoCisterna())
            abrirValvula();

            cerrarValvula();
        }
        else if(sensorLlenoCisterna())
            cerrarValvula();
    }
}
