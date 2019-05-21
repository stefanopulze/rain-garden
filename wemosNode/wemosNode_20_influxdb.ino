void setupInfluxdb()
{
    influx.setDb(INFLUXDB_DATABASE);
}

InfluxData buildInfluxData(String measure, byte sensorId, float value)
{
    InfluxData row(measure);
    char idPadded[2];
    sprintf(idPadded, "%02d", sensorId);
    row.addTag("sensor", "sensor-"+sensorId);
    row.addValue("value", value);
    return row;
}

void sendToInflux(SensorData *data)
{
    InfluxData temperature = buildInfluxData("temperature", data->sensorId, data->temperature);
    influx.prepare(temperature);

    InfluxData humidity = buildInfluxData("humidity", data->sensorId, data->humidity);
    influx.prepare(humidity);

    InfluxData pressure = buildInfluxData("pressure", data->sensorId, data->pressure);
    influx.prepare(humidity);

    // writes all prepared measurements with a single request into db.
    boolean success = influx.write();
}
