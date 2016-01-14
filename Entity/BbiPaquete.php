<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of BbiPaquete
 *
 * @author ifarre
 */
class BbiPaquete {
    private $tituloVial = NULL;
    private $tipoVial = NULL;
    private $localizacion = NULL;
    private $ayuntamiento = NULL;
    private $veterinario = NULL;
    private $tituloChapa = NULL;
    private $tipoChapa = NULL;
    private $nombrePerro = NULL;
    private $digitoControl = NULL;
    private $matching = NULL;
    private $resultado = NULL;
    private $estado = NULL;
    private $fechaVialLleno = NULL;
    private $fechaInforme = NULL;
    private $fechaEnvioChapa = NULL;
    private $genotipo = NULL;
    private $tipoPrueba = NULL;
    private $pdf = NULL;
    
    public function getFechaEnvioChapa() {
        return $this->fechaEnvioChapa;
    }

    public function setFechaEnvioChapa($fechaEnvioChapa) {
        $this->fechaEnvioChapa = $fechaEnvioChapa;
    }

        
    public function getTipoPrueba() {
        return $this->tipoPrueba;
    }

    public function setTipoPrueba($tipoPrueba) {
        $this->tipoPrueba = $tipoPrueba;
    }

        
    public function getLocalizacion() {
        return $this->localizacion;
    }

    public function setLocalizacion($localizacion) {
        $this->localizacion = $localizacion;
    }

    public function getTituloVial() {
        return $this->tituloVial;
    }

    public function getTipoVial() {
        return $this->tipoVial;
    }

    public function getAyuntamiento() {
        return $this->ayuntamiento;
    }

    public function getVeterinario() {
        return $this->veterinario;
    }

    public function getTituloChapa() {
        return $this->tituloChapa;
    }

    public function getTipoChapa() {
        return $this->tipoChapa;
    }

    public function getNombrePerro() {
        return $this->nombrePerro;
    }

    public function getDigitoControl() {
        return $this->digitoControl;
    }

    public function getMatching() {
        return $this->matching;
    }

    public function getResultado() {
        return $this->resultado;
    }

    public function getEstado() {
        return $this->estado;
    }

    public function getFechaVialLleno() {
        return $this->fechaVialLleno;
    }

    public function getFechaInforme() {
        return $this->fechaInforme;
    }

    public function getGenotipo() {
        return $this->genotipo;
    }

    public function getPdf() {
        return $this->pdf;
    }

    public function setTituloVial($tituloVial) {
        $this->tituloVial = $tituloVial;
    }

    public function setTipoVial($tipoVial) {
        $this->tipoVial = $tipoVial;
    }

    public function setAyuntamiento($ayuntamiento) {
        $this->ayuntamiento = $ayuntamiento;
    }

    public function setVeterinario($veterinario) {
        $this->veterinario = $veterinario;
    }

    public function setTituloChapa($tituloChapa) {
        $this->tituloChapa = $tituloChapa;
    }

    public function setTipoChapa($tipoChapa) {
        $this->tipoChapa = $tipoChapa;
    }

    public function setNombrePerro($nombrePerro) {
        $this->nombrePerro = $nombrePerro;
    }

    public function setDigitoControl($digitoControl) {
        $this->digitoControl = $digitoControl;
    }

    public function setMatching($matching) {
        $this->matching = $matching;
    }

    public function setResultado($resultado) {
        $this->resultado = $resultado;
    }

    public function setEstado($estado) {
        $this->estado = $estado;
    }

    public function setFechaVialLleno($fechaVialLleno) {
        $this->fechaVialLleno = $fechaVialLleno;
    }

    public function setFechaInforme($fechaInforme) {
        $this->fechaInforme = $fechaInforme;
    }

    public function setGenotipo($genotipo) {
        $this->genotipo = $genotipo;
    }

    public function setPdf($pdf) {
        $this->pdf = $pdf;
    }


    
}
