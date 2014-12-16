<?php


namespace Mongo;
class Orchestration {
    protected $baseuri;
    protected $conf;

    function __construct($baseuri) {
        $this->baseuri = $baseuri;
        $this->conf = array(
            "timeout" => 35,
        );
    }

    function ping() {
        try {
            $data = $this->get("");
        } catch(\Exception $e) {
            return false;
        }
        return true;
    }

    function stopAll() {
        $servers = $this->get("servers");
        foreach($servers["servers"] as $server) {
            $this->stopServer($server["id"]);
        }
    }

    function hasStandalone() {
        try {
            $data = $this->get("servers/STANDALONE");
        } catch(\Exception $e) {
            return false;
        }
        return $data["mongodb_uri"];
    }

    function startStandalone() {
        $retval = $this->post("servers", ["name" => "mongod", "id" => "STANDALONE"]);
        if ($retval["procInfo"]["alive"]) {
            return $retval["mongodb_uri"];
        }

        return false;
    }

    function stopStandalone() {
        return $this->stopServer("STANDALONE");
    }

    function getServerInfo($id) {
        $data = $this->get("servers/" . $id);
        return $data;
    }

    function stopServer($id) {
        try {
            $retval = $this->delete("servers/$id");
            return true;
        } catch(\Exception $e) {
            if ($e->getCode() == 204) {
                return true;
            }

            return false;
        }
    }
    function getTimeout() {
        return $this->conf["timeout"];
    }

    function delete($target) {
        $opts = [
            "http" => [
                "timeout" => $this->getTimeout(),
                "method"  => "DELETE",
                "header"  => "Accept: application/json\r\n" .
                             "Content-type: application/x-www-form-urlencoded",
                "ignore_errors" => true,
            ],
        ];

        $data = $this->_sendAndReceive($target, $opts);
        return $data;
    }

    function get($target) {
        $opts = [
            "http" => [
                "timeout" => $this->getTimeout(),
                "method"  => "GET",
                "header"  => "Accept: application/json\r\n" .
                             "Content-type: application/x-www-form-urlencoded",
                "ignore_errors" => true,
            ],
        ];

        $data = $this->_sendAndReceive($target, $opts);
        return $data;
    }

    function post($target, $array) {
        $postdata = json_encode($array);

        $opts = [
            "http" => [
                "timeout" => $this->getTimeout(),
                "method"  => "POST",
                "header"  => "Accept: application/json\r\n" .
                             "Content-type: application/x-www-form-urlencoded",
                "content" => $postdata,
                "ignore_errors" => true,
            ],
        ];

        return $this->_sendAndReceive($target, $opts);
    }

    protected function _sendAndReceive($target, $opts) {
        $context = stream_context_create($opts);
        $url = $this->baseuri . "/" . $target;

        $http = $opts["http"];

        $hdr = "";
        foreach(explode("\r\n", $http["header"]) as $header) {
            $hdr .= "--header='{$header}' ";
        }

        $debug = sprintf("wget --body-data='%s' --method='%s' %s %s\n",
            isset($http["content"]) ? $http["content"] : "",
            $http["method"],
            $hdr,
            $url
        );
        if (defined("DUMP_WGET") && DUMP_WGET) {
            echo $debug;
        }

        $data = @file_get_contents($url, false, $context);
        if ($data) {
            return json_decode($data, true);
        }
        if (!empty($http_response_header)) {
            sscanf($http_response_header[0], "HTTP/%f %i %s", $proto, $code, $desc);
            throw new \RuntimeException(json_encode($http_response_header), $code);
        }

        throw new \RuntimeException(error_get_last()["message"]);

    }
}

